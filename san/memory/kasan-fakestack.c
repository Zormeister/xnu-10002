/*
 * Copyright (c) 2016-2020 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */

#include <stdint.h>
#include <stdbool.h>
#include <kern/assert.h>
#include <kern/zalloc.h>
#include <mach/mach_vm.h>
#include <mach/vm_param.h>
#include <libkern/libkern.h>
#include <libkern/OSAtomic.h>
#include <sys/queue.h>
#include <kern/thread.h>
#include <kern/debug.h>

#include "kasan.h"
#include "kasan_internal.h"

/*
 * TODO: this should be converted to the kasan_quarantine() + proper RZ model
 *       with a separate quarantine from regular zones since this can be called
 *       from interrupt context.
 */

int __asan_option_detect_stack_use_after_return = 0;

#define FAKESTACK_HEADER_SZ 64
#define FAKESTACK_NUM_SZCLASS 7

#define FAKESTACK_UNUSED    0 /* waiting to be collected at next gc - forced by clang */
#define FAKESTACK_ALLOCATED 1
#define FAKESTACK_FREED     2

#if KASAN_FAKESTACK

#define FAKESTACK_NORETURN

struct fakestack_header {
	LIST_ENTRY(fakestack_header) list;
	void *site; /* allocation site */
	struct {
		uint8_t flag;
		vm_size_t realsz : 52;
		vm_size_t sz_class : 4;
	};
	uint64_t __pad0;
};
_Static_assert(sizeof(struct fakestack_header) <= FAKESTACK_HEADER_SZ, "fakestack_header size mismatch");

static zone_t fakestack_zones[FAKESTACK_NUM_SZCLASS];
static char fakestack_names[FAKESTACK_NUM_SZCLASS][16];
static const unsigned long fakestack_min = 1 << 6;
static const unsigned long __unused fakestack_max = 1 << 16;

/*
 * Enter a fakestack critical section in a reentrant-safe fashion. Returns true on
 * success with the kasan lock held.
 */
static bool
thread_enter_fakestack(boolean_t *flags)
{
	thread_t cur = current_thread();
	if (cur && kasan_lock_held(cur)) {
		/* current thread is already in kasan - fail */
		return false;
	}
	kasan_lock(flags);
	return true;
}

static volatile long suspend_count;
static const long suspend_threshold = 20;

void
kasan_fakestack_suspend(void)
{
	if (OSIncrementAtomicLong(&suspend_count) == suspend_threshold) {
		__asan_option_detect_stack_use_after_return = 0;
	}
}

void
kasan_fakestack_resume(void)
{
	long orig = OSDecrementAtomicLong(&suspend_count);
	assert(orig >= 0);

	if (fakestack_enabled && orig == suspend_threshold) {
		__asan_option_detect_stack_use_after_return = 1;
	}
}

static bool
ptr_is_on_stack(uptr ptr)
{
	vm_offset_t base = dtrace_get_kernel_stack(current_thread());

	if (ptr >= base && ptr < (base + kernel_stack_size)) {
		return true;
	} else {
		return false;
	}
}

/* free all unused fakestack entries */
void
kasan_fakestack_gc(thread_t thread)
{
	struct fakestack_header *cur, *tmp;
	LIST_HEAD(, fakestack_header) tofree = LIST_HEAD_INITIALIZER(tofree);

	boolean_t flags;
	if (!thread_enter_fakestack(&flags)) {
		panic("expected success entering fakestack");
	}

	/* move the unused objects off the per-thread list... */
	struct fakestack_header_list *head = &kasan_get_thread_data(thread)->fakestack_head;
	LIST_FOREACH_SAFE(cur, head, list, tmp) {
		if (cur->flag == FAKESTACK_UNUSED) {
			LIST_REMOVE(cur, list);
			LIST_INSERT_HEAD(&tofree, cur, list);
			cur->flag = FAKESTACK_FREED;
		}
	}

	kasan_unlock(flags);

	/* ... then actually free them */
	LIST_FOREACH_SAFE(cur, &tofree, list, tmp) {
		/* see TODO at the top */
		kasan_poison_range((vm_offset_t)cur,
		    (fakestack_min << cur->sz_class) + FAKESTACK_HEADER_SZ,
		    ASAN_VALID);
		zfree(fakestack_zones[cur->sz_class], cur);
	}
}

static uint8_t **
fakestack_flag_ptr(vm_offset_t ptr, vm_size_t sz)
{
	uint8_t **x = (uint8_t **)ptr;
	size_t idx = sz / 8;
	return &x[idx - 1];
}

static uptr ALWAYS_INLINE
kasan_fakestack_alloc(int sz_class, size_t realsz)
{
	if (!__asan_option_detect_stack_use_after_return) {
		return 0;
	}

	if (sz_class >= FAKESTACK_NUM_SZCLASS) {
		return 0;
	}

	uptr ret = 0;
	size_t sz = fakestack_min << sz_class;
	assert(realsz <= sz);
	assert(sz <= fakestack_max);
	zone_t zone = fakestack_zones[sz_class];

	boolean_t flags;
	if (!thread_enter_fakestack(&flags)) {
		return 0;
	}

	ret = (uptr)zalloc_noblock(zone);

	if (ret) {
		size_t leftrz = 32 + FAKESTACK_HEADER_SZ;
		size_t validsz = realsz - 32 - 16; /* remove redzones */
		size_t rightrz = sz - validsz - 32; /* 16 bytes, plus whatever is left over */
		struct fakestack_header *hdr = (struct fakestack_header *)ret;

		kasan_poison(ret, validsz, leftrz, rightrz, ASAN_STACK_RZ);

		hdr->site = __builtin_return_address(0);
		hdr->realsz = realsz;
		hdr->sz_class = sz_class;
		hdr->flag = FAKESTACK_ALLOCATED;
		ret += FAKESTACK_HEADER_SZ;

		*fakestack_flag_ptr(ret, sz) = &hdr->flag; /* back ptr to the slot */
		struct fakestack_header_list *head = &kasan_get_thread_data(current_thread())->fakestack_head;
		LIST_INSERT_HEAD(head, hdr, list);
	}

	kasan_unlock(flags);
	return ret;
}

static void NOINLINE
kasan_fakestack_free(int sz_class, uptr dst, size_t realsz)
{
	if (ptr_is_on_stack(dst)) {
		return;
	}

	assert(realsz <= (fakestack_min << sz_class));

	vm_size_t sz = fakestack_min << sz_class;
	zone_t zone = fakestack_zones[sz_class];
	assert(zone);

	/* TODO: check the magic? */

	dst -= FAKESTACK_HEADER_SZ;
	sz += FAKESTACK_HEADER_SZ;

	struct fakestack_header *hdr = (struct fakestack_header *)dst;
	assert(hdr->sz_class == sz_class);

	boolean_t flags;

	kasan_lock(&flags);
	LIST_REMOVE(hdr, list);
	kasan_unlock(flags);

	/* see TODO at the top */
	kasan_poison_range((vm_offset_t)dst, sz, ASAN_VALID);
	zfree(fakestack_zones[sz_class], dst);
}

void NOINLINE
kasan_fakestack_drop(thread_t thread)
{
	boolean_t flags;
	if (!thread_enter_fakestack(&flags)) {
		panic("expected success entering fakestack");
	}

	struct fakestack_header_list *head = &kasan_get_thread_data(thread)->fakestack_head;
	struct fakestack_header *cur;
	LIST_FOREACH(cur, head, list) {
		if (cur->flag == FAKESTACK_ALLOCATED) {
			cur->flag = FAKESTACK_UNUSED;
		}
	}

	kasan_unlock(flags);
}

void NOINLINE
kasan_init_fakestack(void)
{
	/* allocate the fakestack zones */
	for (int i = 0; i < FAKESTACK_NUM_SZCLASS; i++) {
		unsigned long sz = (fakestack_min << i) + FAKESTACK_HEADER_SZ;
		size_t maxsz = 256UL * 1024;

		if (i <= 3) {
			/* size classes 0..3 are much more common */
			maxsz *= 4;
		}

		snprintf(fakestack_names[i], 16, "fakestack.%d", i);
		fakestack_zones[i] = zone_create_ext(fakestack_names[i], sz,
		    ZC_NOCALLOUT | ZC_NOGC | ZC_NOCACHING |
		    ZC_KASAN_NOREDZONE | ZC_KASAN_NOQUARANTINE,
		    ZONE_ID_ANY, ^(zone_t z) {
			zone_set_exhaustible(z, maxsz / sz, true);
		});
		zone_fill_initially(fakestack_zones[i], maxsz / sz);
	}

	/* globally enable */
	if (fakestack_enabled) {
		__asan_option_detect_stack_use_after_return = 1;
	}
}

static int
sysctl_fakestack_enable(__unused struct sysctl_oid *oidp, __unused void *arg1, int __unused arg2, struct sysctl_req *req)
{
	int ch, err, val;

	err = sysctl_io_number(req, fakestack_enabled, sizeof(fakestack_enabled), &val, &ch);
	if (err == 0 && ch) {
		fakestack_enabled = !!val;
		__asan_option_detect_stack_use_after_return = !!val;
	}

	return err;
}

SYSCTL_PROC(_kern_kasan, OID_AUTO, fakestack,
    CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_LOCKED,
    0, 0, sysctl_fakestack_enable, "I", "");

#else /* KASAN_FAKESTACK */

#define FAKESTACK_NORETURN      OS_NORETURN

void
kasan_init_fakestack(void)
{
	assert(__asan_option_detect_stack_use_after_return == 0);
}

void
kasan_unpoison_fakestack(thread_t __unused thread)
{
	assert(__asan_option_detect_stack_use_after_return == 0);
}

void NOINLINE
kasan_fakestack_drop(thread_t __unused thread)
{
	assert(__asan_option_detect_stack_use_after_return == 0);
}

void
kasan_fakestack_gc(thread_t __unused thread)
{
	assert(__asan_option_detect_stack_use_after_return == 0);
}

void
kasan_fakestack_suspend(void)
{
	assert(__asan_option_detect_stack_use_after_return == 0);
}

void
kasan_fakestack_resume(void)
{
	assert(__asan_option_detect_stack_use_after_return == 0);
}

static uptr
kasan_fakestack_alloc(int __unused sz_class, size_t __unused realsz)
{
	assert(__asan_option_detect_stack_use_after_return == 0);
	return 0;
}

static void OS_NORETURN
kasan_fakestack_free(int __unused sz_class, uptr __unused dst, size_t __unused realsz)
{
	assert(__asan_option_detect_stack_use_after_return == 0);
	panic("fakestack_free called on non-FAKESTACK config");
}

#endif

void
kasan_init_thread(struct kasan_thread_data *td)
{
	LIST_INIT(&td->fakestack_head);
}

#define FAKESTACK_DECLARE(szclass) \
	uptr __asan_stack_malloc_##szclass(size_t sz)  { return kasan_fakestack_alloc(szclass, sz); } \
	void FAKESTACK_NORETURN __asan_stack_free_##szclass (uptr dst, size_t sz)  { kasan_fakestack_free(szclass, dst, sz); }

FAKESTACK_DECLARE(0)
FAKESTACK_DECLARE(1)
FAKESTACK_DECLARE(2)
FAKESTACK_DECLARE(3)
FAKESTACK_DECLARE(4)
FAKESTACK_DECLARE(5)
FAKESTACK_DECLARE(6)
FAKESTACK_DECLARE(7)
FAKESTACK_DECLARE(8)
FAKESTACK_DECLARE(9)
FAKESTACK_DECLARE(10)
