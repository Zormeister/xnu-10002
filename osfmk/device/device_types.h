/*
 * Copyright (c) 2000-2004 Apple Computer, Inc. All rights reserved.
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
/*
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 *
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 *
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 *
 * Carnegie Mellon requests users of this software to return to
 *
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 *
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 */
/*
 *	Author: David B. Golub, Carnegie Mellon University
 *	Date:   3/89
 */

#ifndef DEVICE_TYPES_H
#define DEVICE_TYPES_H

/*
 * Types for device interface.
 */
#include <mach/std_types.h>
#include <mach/mach_types.h>
#include <mach/message.h>
#include <mach/port.h>
#ifdef MACH_KERNEL_PRIVATE
#include <mach_debug/mach_debug_types.h>
#endif
#include <stdint.h>

#if PRIVATE
#define IOKIT_SERVER_VERSION    20210810
#endif


/*
 * IO buffer - out-of-line array of characters.
 */
typedef char *  io_buf_ptr_t;

/*
 * Some types for IOKit.
 */

#ifdef IOKIT

/* must match device_types.defs */
typedef char                    io_name_t[128];
typedef char                    io_string_t[512];
typedef char                    io_string_inband_t[4096];
typedef char                    io_struct_inband_t[4096];

#if KERNEL
typedef uint64_t                io_user_scalar_t;
typedef uint64_t                io_user_reference_t __kernel_ptr_semantics;
typedef int                     io_scalar_inband_t[16];
// must be the same type as OSAsyncReference
typedef natural_t               io_async_ref_t[8] __kernel_ptr_semantics;
typedef io_user_scalar_t        io_scalar_inband64_t[16];
typedef io_user_reference_t     io_async_ref64_t[8];
#elif __LP64__
typedef uint64_t                io_user_scalar_t;
typedef uint64_t                io_user_reference_t;
typedef io_user_scalar_t        io_scalar_inband_t[16];
typedef io_user_reference_t     io_async_ref_t[8];
typedef io_user_scalar_t        io_scalar_inband64_t[16];
typedef io_user_reference_t     io_async_ref64_t[8];
#else
typedef int                     io_user_scalar_t;
typedef natural_t               io_user_reference_t;
typedef io_user_scalar_t        io_scalar_inband_t[16];
typedef io_user_reference_t     io_async_ref_t[8];
typedef uint64_t                io_scalar_inband64_t[16];
typedef uint64_t                io_async_ref64_t[8];
#endif // __LP64__

#ifdef MACH_KERNEL_PRIVATE

typedef struct IOObject * io_object_t;
typedef struct IOMachPort * io_kobject_t;
typedef io_object_t io_connect_t;
typedef io_object_t io_ident_t;
typedef io_object_t uext_object_t;

extern void iokit_add_reference( io_object_t obj, natural_t type );
extern void iokit_remove_reference( io_object_t obj );
extern void iokit_remove_connect_reference( io_object_t obj );
extern void iokit_port_object_description(io_object_t obj, kobject_description_t desc);

extern io_object_t iokit_lookup_object_port( ipc_port_t port );
extern io_connect_t iokit_lookup_connect_port( ipc_port_t port );
extern io_ident_t iokit_lookup_ident_port( ipc_port_t port );
extern uext_object_t iokit_lookup_uext_object_port( ipc_port_t port );

extern ipc_port_t iokit_make_object_port( io_object_t obj );
extern ipc_port_t iokit_make_connect_port( io_connect_t obj );
extern ipc_port_t iokit_make_ident_port( io_ident_t obj );

extern void iokit_kobject_retain(io_kobject_t machPort);
extern io_object_t iokit_copy_object_for_consumed_kobject(io_kobject_t machPort, natural_t type);

#else

#ifndef __IOKIT_PORTS_DEFINED__
#define __IOKIT_PORTS_DEFINED__
typedef mach_port_t     io_object_t;
#endif  /* __IOKIT_PORTS_DEFINED__ */

#endif  /* MACH_KERNEL */

#endif  /* IOKIT */

#endif  /* DEVICE_TYPES_H */
