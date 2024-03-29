/*
 * Copyright (c) 2010-2018 Apple Computer, Inc. All rights reserved.
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
/*
 * @OSF_COPYRIGHT@
 */

#ifndef _KERN_LEDGER_H_
#define _KERN_LEDGER_H_

#include <mach/mach_types.h>    /* ledger_t */

#ifdef MACH_KERNEL_PRIVATE
#include <os/refcnt.h>
#endif /* MACH_KERNEL_PRIVATE */

#define LEDGER_INFO             0
#define LEDGER_ENTRY_INFO       1
#define LEDGER_TEMPLATE_INFO    2
#define LEDGER_LIMIT            3
/* LEDGER_MAX_CMD always tracks the index of the last ledger command. */
#define LEDGER_MAX_CMD          LEDGER_LIMIT

#define LEDGER_NAME_MAX 32

struct ledger_info {
	char    li_name[LEDGER_NAME_MAX];
	int64_t li_id;
	int64_t li_entries;
};

struct ledger_template_info {
	char            lti_name[LEDGER_NAME_MAX];
	char            lti_group[LEDGER_NAME_MAX];
	char            lti_units[LEDGER_NAME_MAX];
};

#ifdef MACH_KERNEL_PRIVATE
/*
 * The explicit alignment is to ensure that atomic operations don't panic
 * on ARM.
 */
struct ledger_entry {
	volatile uint32_t le_flags;
#define LEDGER_PERCENT_NONE  UINT16_MAX
#define LEDGER_DIAG_MEM_THRESHOLD_INFINITY ((int16_t)((1ULL << 16) - 1))

	uint16_t                 le_warn_percent;
	int16_t                  le_diag_threshold_scaled;  /* Diag mem threshold for this entry, handled in Mbytes */
	ledger_amount_t          le_limit;
	volatile ledger_amount_t le_credit __attribute__((aligned(8)));
	volatile ledger_amount_t le_debit  __attribute__((aligned(8)));
	union {
		struct {
			/*
			 * XXX - the following two fields can go away if we move all of
			 * the refill logic into process policy
			 */
			uint64_t le_refill_period;
			uint64_t le_last_refill;
		} le_refill;
		struct {
			ledger_amount_t le_lifetime_max; /* Process lifetime peak */
#if CONFIG_LEDGER_INTERVAL_MAX
			ledger_amount_t le_interval_max; /* Interval peak XXX better name needed */
#endif
		} _le_max;
	} _le;
} __attribute__((aligned(8)));

/*
 * Many ledger entries just need to track an amount
 * and have a few flags (panic on negative, active / inactive, etc...).
 * Those entries use this struct to save memory.
 */
struct ledger_entry_small {
	volatile uint32_t les_flags;
	volatile ledger_amount_t les_credit __attribute__((aligned(8)));
} __attribute__((aligned(8)));

struct ledger {
	uint64_t                  l_id;
	os_refcnt_t               l_refs;
	int32_t                   l_size;
	struct ledger_template *  l_template;
	struct ledger_entry_small l_entries[] __attribute__((aligned(8)));
};
#endif /* MACH_KERNEL_PRIVATE */

struct ledger_entry_info {
	int64_t         lei_balance;
	int64_t         lei_credit;
	int64_t         lei_debit;
	uint64_t        lei_limit;
	uint64_t        lei_refill_period;      /* In nanoseconds */
	uint64_t        lei_last_refill;        /* Time since last refill */
};

struct ledger_limit_args {
	char            lla_name[LEDGER_NAME_MAX];
	uint64_t        lla_limit;
	uint64_t        lla_refill_period;
};

#ifdef  KERNEL_PRIVATE

typedef struct ledger_template *ledger_template_t;

#define LEDGER_VALID(ledger)    (ledger != LEDGER_NULL)

/* Action to take when a ledger goes into deficit */
#define LEDGER_ACTION_IGNORE    0x0000
#define LEDGER_ACTION_BLOCK     0x0010
#define LEDGER_ACTION_CALLBACK  0x0020
#define LEDGER_ACTION_MASK      0x00f0

/*
 * Types of warnings that trigger a callback.
 */
#define LEDGER_WARNING_ROSE_ABOVE       1
#define LEDGER_WARNING_DIPPED_BELOW     2
#define LEDGER_WARNING_DIAG_MEM_THRESHOLD  3

typedef void (*ledger_callback_t)(int warning, const void * param0, const void *param1);

extern ledger_template_t ledger_template_create(const char *name);
extern ledger_template_t ledger_template_copy(ledger_template_t template, const char *name);
extern void ledger_template_dereference(ledger_template_t template);
/*
 * DEPRECATED. Use ledger_entry_add_with_flags instead.
 */
extern int ledger_entry_add(ledger_template_t template, const char *key,
    const char *group, const char *units);
extern kern_return_t ledger_set_callback(ledger_template_t template, int entry,
    ledger_callback_t callback, const void *param0, const void *param1);
extern kern_return_t ledger_track_maximum(ledger_template_t template, int entry,
    int period_in_secs);
extern kern_return_t ledger_panic_on_negative(ledger_template_t template,
    int entry);
extern kern_return_t ledger_track_credit_only(ledger_template_t template,
    int entry);
extern int ledger_key_lookup(ledger_template_t template, const char *key);

/*
 * Supported ledger features.
 * Passed in as a bitwise OR to ledger_entry_add_with_flags
 */
__options_decl(ledger_entry_flags, uint64_t, {
	LEDGER_ENTRY_ALLOW_CALLBACK = 0x1,
	LEDGER_ENTRY_ALLOW_MAXIMUM = 0x2,
	LEDGER_ENTRY_ALLOW_PANIC_ON_NEGATIVE = 0x4,
	LEDGER_ENTRY_ALLOW_DEBIT = 0x8,
	LEDGER_ENTRY_ALLOW_LIMIT = 0x10,
	LEDGER_ENTRY_ALLOW_ACTION = 0x20,
	LEDGER_ENTRY_ALLOW_INACTIVE = 0x40,
});

/*
 * Create a new ledger entry that only supports the feature set passed in via
 * flags.
 *
 * This is the recommended way to create new ledger entries, as it allows
 * the ledger code to allocate less memory for simpler entries.
 * ledger_entry_add is kept for backwards compatibility.
 *
 * See below for the set of valid flags.
 */
extern int ledger_entry_add_with_flags(ledger_template_t template, const char *key,
    const char *group, const char *units, ledger_entry_flags flags);

/* value of entry type */
#define LEDGER_CREATE_ACTIVE_ENTRIES    0
#define LEDGER_CREATE_INACTIVE_ENTRIES  1
extern ledger_t ledger_instantiate(ledger_template_t template, int entry_type);
extern void ledger_template_complete(ledger_template_t template);
extern void ledger_template_complete_secure_alloc(ledger_template_t template);
extern kern_return_t ledger_disable_callback(ledger_t ledger, int entry);
extern kern_return_t ledger_enable_callback(ledger_t ledger, int entry);
extern kern_return_t ledger_get_limit(ledger_t ledger, int entry,
    ledger_amount_t *limit);
extern kern_return_t ledger_set_limit(ledger_t ledger, int entry,
    ledger_amount_t limit, uint8_t warn_level_percentage);
#if CONFIG_LEDGER_INTERVAL_MAX
extern kern_return_t ledger_get_interval_max(ledger_t ledger, int entry,
    ledger_amount_t *max_interval_balance, int reset);
#endif /* CONFIG_LEDGER_INTERVAL_MAX */
extern kern_return_t ledger_get_lifetime_max(ledger_t ledger, int entry,
    ledger_amount_t *max_lifetime_balance);
extern kern_return_t ledger_get_actions(ledger_t ledger, int entry, int *actions);
extern kern_return_t ledger_set_action(ledger_t ledger, int entry, int action);
extern kern_return_t ledger_get_period(ledger_t ledger, int entry,
    uint64_t *period);
extern kern_return_t ledger_set_period(ledger_t ledger, int entry,
    uint64_t period);
extern kern_return_t ledger_disable_refill(ledger_t l, int entry);
extern kern_return_t ledger_entry_setactive(ledger_t ledger, int entry);
extern void ledger_check_new_balance(thread_t thread, ledger_t ledger, int entry);
extern kern_return_t ledger_credit(ledger_t ledger, int entry,
    ledger_amount_t amount);
extern kern_return_t ledger_credit_nocheck(ledger_t ledger, int entry,
    ledger_amount_t amount);
extern kern_return_t ledger_debit(ledger_t ledger, int entry,
    ledger_amount_t amount);
extern kern_return_t ledger_debit_nocheck(ledger_t ledger, int entry,
    ledger_amount_t amount);
extern kern_return_t ledger_credit_thread(thread_t thread, ledger_t ledger,
    int entry, ledger_amount_t amount);
extern kern_return_t ledger_debit_thread(thread_t thread, ledger_t ledger,
    int entry, ledger_amount_t amount);
extern kern_return_t ledger_zero_balance(ledger_t ledger, int entry);
extern kern_return_t ledger_get_entries(ledger_t ledger, int entry,
    ledger_amount_t *credit, ledger_amount_t *debit);
extern kern_return_t ledger_get_balance(ledger_t ledger, int entry,
    ledger_amount_t *balance);
extern kern_return_t ledger_reset_callback_state(ledger_t ledger, int entry);
extern kern_return_t ledger_disable_panic_on_negative(ledger_t ledger, int entry);
extern kern_return_t ledger_get_panic_on_negative(ledger_t ledger, int entry, int *panic_on_negative);

extern kern_return_t ledger_rollup(ledger_t to_ledger, ledger_t from_ledger);
extern kern_return_t ledger_rollup_entry(ledger_t to_ledger, ledger_t from_ledger, int entry);

extern void ledger_ast(thread_t thread);

extern void ledger_reference(ledger_t ledger);
extern void ledger_dereference(ledger_t ledger);

extern ledger_amount_t ledger_get_remaining(ledger_t ledger, int entry);
extern void ledger_restart(ledger_t ledger, int entry, uint64_t now);
extern uint64_t ledger_get_interval_remaining(ledger_t ledger, int entry, uint64_t now);

/* Support for ledger() syscall */
#ifdef LEDGER_DEBUG
extern int ledger_limit(task_t task, struct ledger_limit_args *args);
#endif
extern int ledger_info(task_t task, struct ledger_info *info);

extern int
ledger_get_task_entry_info_multiple(task_t task, void **buf, int *len);

extern void
ledger_get_entry_info(ledger_t ledger, int entry,
    struct ledger_entry_info *lei);

extern int ledger_template_info(void **buf, int *len);

#if DEBUG || DEVELOPMENT
extern kern_return_t ledger_get_diag_mem_threshold(ledger_t ledger, int entry,
    ledger_amount_t *limit);

extern kern_return_t ledger_set_diag_mem_threshold(ledger_t ledger, int entry,
    ledger_amount_t limit);
extern kern_return_t ledger_set_diag_mem_threshold_disabled(ledger_t ledger, int entry);
extern kern_return_t ledger_set_diag_mem_threshold_enabled(ledger_t ledger, int entry);
extern kern_return_t ledger_is_diag_threshold_enabled(ledger_t ledger, int entry, bool *status);
#endif // DEBUG || DEVELOPMENT

#endif /* KERNEL_PRIVATE */

#endif  /* _KERN_LEDGER_H_ */
