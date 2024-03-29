/*
 * Copyright (c) 2010 Apple Inc. All rights reserved.
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

#include <TargetConditionals.h>
#include <stdbool.h>
#include <strings.h>
#include <unistd.h>
#include <mach/vm_page_size.h>
#include <spawn_filtering_private.h>
#include "_libkernel_init.h"
#include "system-version-compat-support.h"

extern int mach_init(void);

#if SYSTEM_VERSION_COMPAT_ENABLED

#include <sys/sysctl.h>

extern bool _system_version_compat_check_path_suffix(const char *orig_path);
extern int _system_version_compat_open_shim(int opened_fd, int openat_fd, const char *orig_path, int oflag, mode_t mode,
    int (*close_syscall)(int), int (*open_syscall)(const char *, int, mode_t),
    int (*openat_syscall)(int, const char *, int, mode_t),
    int (*fcntl_syscall)(int, int, long));

extern bool (*system_version_compat_check_path_suffix)(const char *orig_path);
extern int (*system_version_compat_open_shim)(int opened_fd, int openat_fd, const char *orig_path, int oflag, mode_t mode,
    int (*close_syscall)(int), int (*open_syscall)(const char *, int, mode_t),
    int (*openat_syscall)(int, const char *, int, mode_t),
    int (*fcntl_syscall)(int, int, long));

extern system_version_compat_mode_t system_version_compat_mode;

int  __sysctlbyname(const char *name, size_t namelen, void *oldp, size_t *oldlenp, void *newp, size_t newlen);
#endif /* SYSTEM_VERSION_COMPAT_ENABLED */

#if POSIX_SPAWN_FILTERING_ENABLED
struct _posix_spawn_args_desc;
extern bool (*posix_spawn_with_filter)(pid_t *pid, const char *fname, char * const *argp,
    char * const *envp, struct _posix_spawn_args_desc *adp, int *ret);
extern bool _posix_spawn_with_filter(pid_t *pid, const char *fname, char * const *argp,
    char * const *envp, struct _posix_spawn_args_desc *adp, int *ret);
extern int (*execve_with_filter)(char *fname, char **argp, char **envp);
extern int _execve_with_filter(char *fname, char **argp, char **envp);
#endif /* POSIX_SPAWN_FILTERING_ENABLED */

#if TARGET_OS_OSX
__attribute__((visibility("default")))
extern bool _os_xbs_chrooted;
bool _os_xbs_chrooted;
#endif /* TARGET_OS_OSX */

/* dlsym() funcptr is for legacy support in exc_catcher */
void* (*LIBKERNEL_FUNCTION_PTRAUTH(_dlsym))(void*, const char*) __attribute__((visibility("hidden")));

__attribute__((visibility("hidden")))
_libkernel_functions_t _libkernel_functions;


void
__libkernel_init(_libkernel_functions_t fns,
    const char *envp[] __attribute__((unused)),
    const char *apple[] __attribute__((unused)),
    const struct ProgramVars *vars __attribute__((unused)))
{
	_libkernel_functions = fns;
	if (fns->dlsym) {
		_dlsym = fns->dlsym;
	}
	mach_init();
#if TARGET_OS_OSX
	for (size_t i = 0; envp[i]; i++) {

#if defined(__i386__) || defined(__x86_64__)
		const char *VM_KERNEL_PAGE_SHIFT_ENV = "VM_KERNEL_PAGE_SIZE_4K=1";
		if (vm_kernel_page_shift != 12 && strcmp(VM_KERNEL_PAGE_SHIFT_ENV, envp[i]) == 0) {
			vm_kernel_page_shift = 12;
			vm_kernel_page_size = 1 << vm_kernel_page_shift;
			vm_kernel_page_mask = vm_kernel_page_size - 1;
		}
#endif /* defined(__i386__) || defined(__x86_64__) */
	}
#endif /* TARGET_OS_OSX */
}

void
__libkernel_init_late(_libkernel_late_init_config_t config)
{
	if (config->version >= 1) {
#if SYSTEM_VERSION_COMPAT_ENABLED
#if TARGET_OS_OSX && !defined(__i386__)
		if (config->enable_system_version_compat) {
			/* enable the version compatibility shim for this process (macOS only) */

			/* first hook up the shims we reference from open{at}() */
			system_version_compat_check_path_suffix = _system_version_compat_check_path_suffix;
			system_version_compat_open_shim = _system_version_compat_open_shim;

			system_version_compat_mode = SYSTEM_VERSION_COMPAT_MODE_MACOSX;

			/*
			 * tell the kernel the shim is enabled for this process so it can shim any
			 * necessary sysctls
			 */
			int enable = 1;
			__sysctlbyname("kern.system_version_compat", strlen("kern.system_version_compat"),
			    NULL, NULL, &enable, sizeof(enable));
		} else if ((config->version >= 2) && config->enable_ios_version_compat) {
			/* enable the iOS ProductVersion compatibility shim for this process */

			/* first hook up the shims we reference from open{at}() */
			system_version_compat_check_path_suffix = _system_version_compat_check_path_suffix;
			system_version_compat_open_shim = _system_version_compat_open_shim;

			system_version_compat_mode = SYSTEM_VERSION_COMPAT_MODE_IOS;

			/*
			 * We don't currently shim any sysctls for iOS apps running on macOS so we
			 * don't need to inform the kernel that this app has the SystemVersion shim enabled.
			 */
		}
#endif /* TARGET_OS_OSX && !defined(__i386__) */

#endif /* SYSTEM_VERSION_COMPAT_ENABLED */

#if POSIX_SPAWN_FILTERING_ENABLED
		if ((config->version >= 3) && config->enable_posix_spawn_filtering) {
			posix_spawn_with_filter = _posix_spawn_with_filter;
			execve_with_filter = _execve_with_filter;
		}
#endif /* POSIX_SPAWN_FILTERING_ENABLED */
	}
}

void
__libkernel_init_after_boot_tasks(
	_libkernel_init_after_boot_tasks_config_t config)
{
	if (config->version >= 1) {
#if POSIX_SPAWN_FILTERING_ENABLED
		if (config->enable_posix_spawn_filtering) {
			posix_spawn_with_filter = _posix_spawn_with_filter;
			execve_with_filter = _execve_with_filter;
		}
#endif /* POSIX_SPAWN_FILTERING_ENABLED */
	}
}
