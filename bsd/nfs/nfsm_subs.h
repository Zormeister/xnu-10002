/*
 * Copyright (c) 2000-2011 Apple Inc. All rights reserved.
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
/* Copyright (c) 1995 NeXT Computer, Inc. All Rights Reserved */
/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Rick Macklem at The University of Guelph.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)nfsm_subs.h	8.2 (Berkeley) 3/30/95
 * FreeBSD-Id: nfsm_subs.h,v 1.13 1997/07/16 09:06:30 dfr Exp $
 */


#ifndef _NFS_NFSM_SUBS_H_
#define _NFS_NFSM_SUBS_H_

#include <sys/appleapiopts.h>

#ifdef __APPLE_API_PRIVATE

#include <nfs/nfs_conf.h>

int nfsm_chain_new_mbuf(struct nfsm_chain *, size_t);
int nfsm_chain_add_opaque_f(struct nfsm_chain *, const u_char *, size_t);
int nfsm_chain_add_opaque_nopad_f(struct nfsm_chain *, const u_char *, size_t);

int nfsm_chain_advance(struct nfsm_chain *, size_t);
size_t nfsm_chain_offset(struct nfsm_chain *);
int nfsm_chain_get_opaque_pointer_f(struct nfsm_chain *, uint32_t, u_char **);
int nfsm_chain_get_opaque_f(struct nfsm_chain *, size_t, u_char *);
int nfsm_chain_get_uio(struct nfsm_chain *, size_t, uio_t);

#if CONFIG_NFS_SERVER
void nfsm_adj(mbuf_t, int, int);
int nfsm_mbuf_get_list(size_t, mbuf_t *, int *);

int nfsm_chain_add_fattr(struct nfsrv_descript *, struct nfsm_chain *, struct vnode_attr *);
int nfsm_chain_add_wcc_data_f(struct nfsrv_descript *, struct nfsm_chain *, int,
    struct vnode_attr *, int, struct vnode_attr *);
int nfsm_chain_get_path_namei(struct nfsm_chain *, uint32_t, struct nameidata *);
int nfsm_chain_get_sattr(struct nfsrv_descript *, struct nfsm_chain *, struct vnode_attr *);
int nfsm_chain_trim_data(struct nfsm_chain *, int, int *);
#endif /* CONFIG_NFS_SERVER */

/* check name length */
#define nfsm_name_len_check(E, ND, LEN) \
	do { \
	        if (E) break; \
	        if (((ND)->nd_vers == NFS_VER2) && ((LEN) > NFS_MAXNAMLEN)) \
	                (E) = NFSERR_NAMETOL; \
	        if ((LEN) <= 0) \
	                error = EBADRPC; \
	} while (0)

#define nfsm_assert(E, COND, ERR) \
	do { \
	        if (E) break; \
	        if (!(COND)) \
	                (E) = (ERR); \
	} while (0)

/* Initialize a vnode_attr to retrieve attributes for the NFS server. */
#define nfsm_srv_vattr_init(VAP, VERS) \
	do { \
	        VATTR_INIT(VAP); \
	        VATTR_WANTED((VAP), va_type); \
	        VATTR_WANTED((VAP), va_mode); \
	        VATTR_WANTED((VAP), va_nlink); \
	        VATTR_WANTED((VAP), va_uid); \
	        VATTR_WANTED((VAP), va_gid); \
	        VATTR_WANTED((VAP), va_data_size); \
	        VATTR_WANTED((VAP), va_data_alloc); \
	        VATTR_WANTED((VAP), va_rdev); \
	        VATTR_WANTED((VAP), va_fsid); \
	        VATTR_WANTED((VAP), va_fileid); \
	        VATTR_WANTED((VAP), va_access_time); \
	        VATTR_WANTED((VAP), va_modify_time); \
	        VATTR_WANTED((VAP), va_change_time); \
	        if ((VERS) == NFS_VER2) \
	                VATTR_WANTED((VAP), va_iosize); \
	        if ((VERS) == NFS_VER3) \
	                VATTR_WANTED((VAP), va_filerev); \
	} while (0)

/* Initialize a vnode_attr to retrieve pre-operation attributes for the NFS server. */
#define nfsm_srv_pre_vattr_init(VAP) \
	do { \
	        VATTR_INIT(VAP); \
	        VATTR_WANTED((VAP), va_data_size); \
	        VATTR_WANTED((VAP), va_modify_time); \
	        VATTR_WANTED((VAP), va_change_time); \
	} while (0)

/* round up to a multiple of 4 */
#define nfsm_rndup(a)   (((a)+3)&(~0x3))

#define nfsm_pad(a)     (nfsm_rndup(a) - (a))

/*
 * control flow macros:
 * go to the appropriate label on condition
 */
#define nfsmout_if(E)   do { if (E) goto nfsmout; } while (0)
#define nfsmerr_if(E)   do { if (E) goto nfsmerr; } while (0)

/*
 * For NFS v2 errors and EBADRPC, the reply contains only the error.
 * This macro is used to skip any reply building code and go straight
 * to nfsmout instead.
 */
#define nfsmout_on_status(ND, E) \
	do { \
	        if (((ND)->nd_repstat == EBADRPC) || \
	            ((ND)->nd_repstat && ((ND)->nd_vers == NFS_VER2))) { \
	                (E) = 0; \
	                goto nfsmout; \
	        } \
	} while (0)

/* initialize an mbuf chain */
#define nfsm_chain_null(NMC) \
	do { \
	        (NMC)->nmc_mhead = (NMC)->nmc_mcur = NULL; \
	        (NMC)->nmc_ptr = NULL; \
	} while (0)

/* cleanup an mbuf chain */
#define nfsm_chain_cleanup(NMC) \
	do { \
	        if (!(NMC)->nmc_mhead) break; \
	        mbuf_freem((NMC)->nmc_mhead); \
	        nfsm_chain_null(NMC); \
	} while (0)

/* get an mbuf given a size hint */
#define nfsm_mbuf_get(E, MBP, SIZEHINT) \
	do { \
	        *(MBP) = NULL; \
	        if ((size_t)(SIZEHINT) >= nfs_mbuf_minclsize) \
	                (E) = mbuf_mclget(MBUF_WAITOK, MBUF_TYPE_DATA, (MBP)); \
	        else \
	                (E) = mbuf_get(MBUF_WAITOK, MBUF_TYPE_DATA, (MBP)); \
	} while (0)

/*
 * get an mbuf with size of M16KCLBYTES given a size hint
 * According to mbuf_getcluster() documentation, clusters greater than 4096 bytes might
 * not be available in all configurations; the caller must additionally check for ENOTSUP.
 * */
#define nfsm_mbuf_getcluster(E, MBP, SIZEHINT) \
	do { \
	        *(MBP) = NULL; \
	        if ((size_t)(SIZEHINT) > MBIGCLBYTES) { \
	                (E) = mbuf_getcluster(MBUF_WAITOK, MBUF_TYPE_DATA, M16KCLBYTES, (MBP)); \
	                if ((E) == 0) { \
	                      break; \
	                } \
	        } \
	        if ((size_t)(SIZEHINT) > MCLBYTES) { \
	                (E) = mbuf_getcluster(MBUF_WAITOK, MBUF_TYPE_DATA, MBIGCLBYTES, (MBP)); \
	                if ((E) == 0) { \
	                      break; \
	                } \
	        } \
	        nfsm_mbuf_get(E, MBP, SIZEHINT); \
	} while (0)


/*
 * macros for building NFS mbuf chains
 */

/* prepare an mbuf chain for building starting with the given mbuf */
#define nfsm_chain_init(NMC, MB) \
	do { \
	        (NMC)->nmc_mhead = (MB); \
	        (NMC)->nmc_mcur = (NMC)->nmc_mhead; \
	        (NMC)->nmc_ptr = mbuf_data((NMC)->nmc_mcur); \
	        (NMC)->nmc_left = mbuf_trailingspace((NMC)->nmc_mcur); \
	        (NMC)->nmc_flags = 0; \
	} while (0)

/* prepare an mbuf chain for building starting with a newly allocated mbuf */
#define nfsm_chain_build_alloc_init(E, NMC, SIZEHINT) \
	do { \
	        mbuf_t ncbimb; \
	        nfsm_mbuf_get((E), &ncbimb, (SIZEHINT)); \
	        if (E) break; \
	        nfsm_chain_init((NMC), ncbimb); \
	} while (0)

/* done building an mbuf chain */
#define nfsm_chain_build_done(E, NMC) \
	do { \
	        if ((E) || !(NMC)->nmc_mcur) break; \
	/* cap off current mbuf */ \
	        mbuf_setlen((NMC)->nmc_mcur, \
	                (NMC)->nmc_ptr - (caddr_t)mbuf_data((NMC)->nmc_mcur)); \
	} while (0)

/* make sure there's room for size bytes in current mbuf */
#define nfsm_chain_check_size(E, NMC, SIZE) \
	do { \
	        if (E) break; \
	        if ((NMC)->nmc_left < (SIZE)) { \
	                (E) = nfsm_chain_new_mbuf((NMC), (SIZE)); \
	                if (!(E) && ((NMC)->nmc_left < (SIZE))) \
	                        (E) = ENOMEM; \
	        } \
	} while (0)

/* add a 32bit value to an mbuf chain extending if necessary */
#define nfsm_chain_add_32(E, NMC, VAL) \
	do { \
	        nfsm_chain_check_size((E), (NMC), NFSX_UNSIGNED); \
	        if (E) break; \
	        *((uint32_t*)(NMC)->nmc_ptr) = txdr_unsigned(VAL); \
	        (NMC)->nmc_ptr += NFSX_UNSIGNED; \
	        (NMC)->nmc_left -= NFSX_UNSIGNED; \
	} while (0)

/* add a 64bit value to an mbuf chain */
#define nfsm_chain_add_64(E, NMC, VAL) \
	do { \
	        uint64_t __tmp64; \
	        nfsm_chain_check_size((E), (NMC), 2 * NFSX_UNSIGNED); \
	        if (E) break; \
	        __tmp64 = (VAL); \
	        txdr_hyper(&__tmp64, (NMC)->nmc_ptr); \
	        (NMC)->nmc_ptr += 2 * NFSX_UNSIGNED; \
	        (NMC)->nmc_left -= 2 * NFSX_UNSIGNED; \
	} while (0)

/* zero the last 4 bytes for a range of opaque */
/* data to make sure any pad bytes will be zero. */
#define nfsm_chain_zero_opaque_pad(BUF, LEN) \
	do { \
	        if ((LEN) > 0) \
	                *(((uint32_t*)(BUF))+((nfsm_rndup(LEN)>>2)-1)) = 0; \
	} while (0)

/* add buffer of opaque data to an mbuf chain */
#define nfsm_chain_add_opaque(E, NMC, BUF, LEN) \
	do { \
	        size_t rndlen = nfsm_rndup(LEN); \
	        if (E) break; \
	        if ((NMC)->nmc_left < rndlen) { \
	                (E) = nfsm_chain_add_opaque_f((NMC), (const u_char*)(BUF), (LEN)); \
	                break; \
	        } \
	        nfsm_chain_zero_opaque_pad((NMC)->nmc_ptr, (LEN)); \
	        bcopy((BUF), (NMC)->nmc_ptr, (LEN)); \
	        (NMC)->nmc_ptr += rndlen; \
	        (NMC)->nmc_left -= rndlen; \
	} while (0)

/* add buffer of opaque data to an mbuf chain without padding */
#define nfsm_chain_add_opaque_nopad(E, NMC, BUF, LEN) \
	do { \
	        if (E) break; \
	        if ((NMC)->nmc_left < (uint32_t) (LEN)) { \
	                (E) = nfsm_chain_add_opaque_nopad_f((NMC), (const u_char*)(BUF), (LEN)); \
	                break; \
	        } \
	        bcopy((BUF), (NMC)->nmc_ptr, (LEN)); \
	        (NMC)->nmc_ptr += (LEN); \
	        (NMC)->nmc_left -= (LEN); \
	} while (0)

/* finish an mbuf in a chain to allow subsequent insertion */
#define nfsm_chain_finish_mbuf(E, NMC) \
	do { \
	        if (E) break; \
	        mbuf_setlen((NMC)->nmc_mcur, \
	                (NMC)->nmc_ptr - (caddr_t)mbuf_data((NMC)->nmc_mcur)); \
	        (NMC)->nmc_left = 0; \
	} while (0)

/* add a file handle to an mbuf chain */
#define nfsm_chain_add_fh(E, NMC, VERS, FHP, FHLEN) \
	do { \
	        if (E) break; \
	        if ((VERS) != NFS_VER2) \
	                nfsm_chain_add_32((E), (NMC), (FHLEN)); \
	        nfsm_chain_add_opaque((E), (NMC), (FHP), (FHLEN)); \
	} while (0)

/* add a string to an mbuf chain */
#define nfsm_chain_add_string(E, NMC, STR, LEN) \
	do { \
	        nfsm_chain_add_32((E), (NMC), (LEN)); \
	        nfsm_chain_add_opaque((E), (NMC), (STR), (LEN)); \
	} while (0)

/* add an NFSv2 time to an mbuf chain */
#define nfsm_chain_add_v2time(E, NMC, TVP) \
	do { \
	        if (TVP) { \
	                nfsm_chain_add_32((E), (NMC), (TVP)->tv_sec); \
	                nfsm_chain_add_32((E), (NMC), ((TVP)->tv_nsec != -1) ? \
	                        ((uint32_t)(TVP)->tv_nsec / 1000) : 0xffffffff); \
	        } else { \
	/* no time... use -1 */ \
	                nfsm_chain_add_32((E), (NMC), -1); \
	                nfsm_chain_add_32((E), (NMC), -1); \
	        } \
	} while (0)

/* add an NFSv3 time to an mbuf chain */
#define nfsm_chain_add_v3time(E, NMC, TVP) \
	do { \
	        nfsm_chain_add_32((E), (NMC), (TVP)->tv_sec); \
	        nfsm_chain_add_32((E), (NMC), (TVP)->tv_nsec); \
	} while (0)

/* add an NFS v2 or v3 time to an mbuf chain */
#define nfsm_chain_add_time(E, NMC, VERS, TVP) \
	do { \
	        if ((VERS) == NFS_VER2) { \
	                nfsm_chain_add_v2time((E), (NMC), (TVP)); \
	        } else { \
	                nfsm_chain_add_v3time((E), (NMC), (TVP)); \
	        } \
	} while (0)

/* add an NFSv3 postop file handle to an mbuf chain */
#define nfsm_chain_add_postop_fh(E, NMC, FHP, FHLEN) \
	do { \
	        nfsm_chain_add_32((E), (NMC), TRUE); \
	        nfsm_chain_add_fh((E), (NMC), NFS_VER3, (FHP), (FHLEN)); \
	} while (0)

/* add NFSv3 postop attributes to an mbuf chain */
#define nfsm_chain_add_postop_attr(E, ND, NMC, ATTRERR, VAP) \
	do { \
	        if (E) break; \
	        if (ATTRERR) { \
	                nfsm_chain_add_32((E), (NMC), FALSE); \
	                break; \
	        } \
	        nfsm_chain_add_32((E), (NMC), TRUE); \
	        if (E) break; \
	        (E) = nfsm_chain_add_fattr((ND), (NMC), (VAP)); \
	} while (0)

/* add NFSv3 WCC data to an mbuf chain */
#define nfsm_chain_add_wcc_data(E, ND, NMC, PREERR, PREVAP, POSTERR, POSTVAP) \
	do { \
	        if (E) break; \
	        (E) = nfsm_chain_add_wcc_data_f((ND), (NMC), \
	                (PREERR), (PREVAP), (POSTERR), (POSTVAP)); \
	} while (0)

/*
 * macros for dissecting NFS mbuf chains
 */

/* prepare an mbuf chain for dissection starting with the given mbuf */
#define nfsm_chain_dissect_init(E, NMC, H) \
	do { \
	        if (!(H)) { \
	                (E) = EINVAL; \
	                break; \
	        } \
	        (NMC)->nmc_mcur = (NMC)->nmc_mhead = (H); \
	        (NMC)->nmc_ptr = mbuf_data(H); \
	        (NMC)->nmc_left = mbuf_len(H); \
	} while (0)

/* skip a number of bytes in an mbuf chain */
#define nfsm_chain_adv(E, NMC, LEN) \
	do { \
	        if (E) break; \
	        if ((NMC)->nmc_left >= (uint32_t)(LEN)) { \
	                (NMC)->nmc_left -= (LEN); \
	                (NMC)->nmc_ptr += (LEN); \
	        } else { \
	                (E) = nfsm_chain_advance((NMC), (LEN)); \
	        } \
	} while (0)

/* get a 32bit value from an mbuf chain */
#define nfsm_chain_get_32(E, NMC, LVAL) \
	do { \
	        uint32_t __tmp32, *__tmpptr; \
	        (LVAL) = 0; \
	        if (E) break; \
	        if ((NMC)->nmc_left >= NFSX_UNSIGNED) { \
	                __tmpptr = (uint32_t*)(NMC)->nmc_ptr; \
	                (NMC)->nmc_left -= NFSX_UNSIGNED; \
	                (NMC)->nmc_ptr += NFSX_UNSIGNED; \
	        } else { \
	                __tmpptr = &__tmp32; \
	                (E) = nfsm_chain_get_opaque_f((NMC), NFSX_UNSIGNED, (u_char*)__tmpptr); \
	                if (E) break; \
	        } \
	        (LVAL) = fxdr_unsigned(uint32_t, *__tmpptr); \
	} while (0)

/* get a 64bit value from an mbuf chain */
#define nfsm_chain_get_64(E, NMC, LVAL) \
	do { \
	        uint64_t __tmp64, *__tmpptr; \
	        (LVAL) = 0; \
	        if (E) break; \
	        if ((NMC)->nmc_left >= 2 * NFSX_UNSIGNED) { \
	                __tmpptr = (uint64_t*)(NMC)->nmc_ptr; \
	                (NMC)->nmc_left -= 2 * NFSX_UNSIGNED; \
	                (NMC)->nmc_ptr += 2 * NFSX_UNSIGNED; \
	        } else { \
	                __tmpptr = &__tmp64; \
	                (E) = nfsm_chain_get_opaque_f((NMC), 2 * NFSX_UNSIGNED, (u_char*)__tmpptr); \
	                if (E) break; \
	        } \
	        fxdr_hyper(__tmpptr, &(LVAL)); \
	} while (0)

/* get a pointer to the next consecutive bytes in an mbuf chain */
#define nfsm_chain_get_opaque_pointer(E, NMC, LEN, PTR) \
	do { \
	        uint32_t rndlen; \
	        if (E) break; \
	        rndlen = nfsm_rndup(LEN); \
	        if (rndlen < (LEN)) { \
	                (E) = EBADRPC; \
	                break; \
	        } \
	        if ((NMC)->nmc_left >= rndlen) { \
	                (PTR) = (void*)(NMC)->nmc_ptr; \
	                (NMC)->nmc_left -= rndlen; \
	                (NMC)->nmc_ptr += rndlen; \
	        } else { \
	                (E) = nfsm_chain_get_opaque_pointer_f((NMC), (LEN), (u_char**)&(PTR)); \
	        } \
	} while (0)

/* copy the next consecutive bytes of opaque data from an mbuf chain */
#define nfsm_chain_get_opaque(E, NMC, LEN, PTR) \
	do { \
	        size_t rndlen; \
	        if (E) break; \
	        rndlen = nfsm_rndup(LEN); \
	        if (rndlen < (LEN)) { \
	                (E) = EBADRPC; \
	                break; \
	        } \
	        if ((NMC)->nmc_left >= rndlen) { \
	                u_char *__tmpptr = (u_char*)(NMC)->nmc_ptr; \
	                (NMC)->nmc_left -= rndlen; \
	                (NMC)->nmc_ptr += rndlen; \
	                bcopy(__tmpptr, (PTR), (LEN)); \
	        } else { \
	                (E) = nfsm_chain_get_opaque_f((NMC), (LEN), (u_char*)(PTR)); \
	        } \
	} while (0)

/* get the size of and a pointer to a file handle in an mbuf chain */
#define nfsm_chain_get_fh_ptr(E, NMC, VERS, FHP, FHSIZE) \
	do { \
	        if ((VERS) != NFS_VER2) { \
	                nfsm_chain_get_32((E), (NMC), (FHSIZE)); \
	                if (E) break; \
	                if ((FHSIZE) > NFSV3_MAX_FH_SIZE) \
	                        (E) = NFSERR_BADHANDLE; \
	        } else \
	                (FHSIZE) = NFSX_V2FH;\
	        if ((E) == 0) \
	                nfsm_chain_get_opaque_pointer((E), (NMC), (FHSIZE), (FHP));\
	} while (0)

/* get the size of and data for a file handle in an mbuf chain */
#define nfsm_chain_get_fh(E, NMC, VERS, FHP) \
	do { \
	        if ((VERS) != NFS_VER2) { \
	                nfsm_chain_get_32((E), (NMC), (FHP)->fh_len); \
	                if ((FHP)->fh_len > sizeof((FHP)->fh_data)) \
	                        (E) = EBADRPC; \
	        } else \
	                (FHP)->fh_len = NFSX_V2FH;\
	        if ((E) == 0) \
	                nfsm_chain_get_opaque((E), (NMC), (uint32_t)(FHP)->fh_len, (FHP)->fh_data);\
	        else \
	                (FHP)->fh_len = 0;\
	} while (0)

/* get an NFS v2 or v3 time from an mbuf chain */
#define nfsm_chain_get_time(E, NMC, VERS, TSEC, TNSEC) \
	do { \
	        nfsm_chain_get_32((E), (NMC), (TSEC)); \
	        nfsm_chain_get_32((E), (NMC), (TNSEC)); \
	        if ((E) || ((VERS) != NFS_VER2)) break; \
	        if ((uint32_t)(TNSEC) == 0xffffffff) \
	                (TNSEC) = 0; \
	        else \
	                (TNSEC) *= 1000; \
	} while (0)

#endif /* __APPLE_API_PRIVATE */
#endif /* _NFS_NFSM_SUBS_H_ */
