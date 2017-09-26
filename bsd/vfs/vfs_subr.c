 * Copyright (c) 2000-2017 Apple Inc. All rights reserved.
#include <machine/machine_routines.h>
#if !CONFIG_EMBEDDED
#endif
#include <vfs/vfs_disk_conditioner.h>
#include <libkern/section_keywords.h>

extern int paniclog_append_noflush(const char *format, ...);

#if !CONFIG_EMBEDDED
		paniclog_append_noflush("mp = %p, phys = %p, prev (%p: %p-%p)\n",
		paniclog_append_noflush("mp = %p, phys = %p, prev (!)\n", hook->mp, phys);
		paniclog_append_noflush("vp = %p, phys = %p, prev (%p: %p-%p)\n",
		paniclog_append_noflush("vp = %p, phys = %p, prev (!)\n", hook->vp, phys);
#endif //CONFIG_EMBEDDED
	/*
	 * The mount iterate mutex is held for the duration of the iteration.
	 * This can be done by a state flag on the mount structure but we can
	 * run into priority inversion issues sometimes.
	 * Using a mutex allows us to benefit from the priority donation
	 * mechanisms in the kernel for locks. This mutex should never be
	 * acquired in spin mode and it should be acquired before attempting to
	 * acquire the mount lock.
	 */
	mount_iterate_lock(mp);

	/* If it returns 0 then there is nothing to do */
		mount_iterate_unlock(mp);
#if !CONFIG_EMBEDDED
#endif
#if !CONFIG_EMBEDDED
#endif
#if !CONFIG_EMBEDDED
#endif
	mount_iterate_unlock(mp);
void
mount_iterate_lock(mount_t mp)
{
	lck_mtx_lock(&mp->mnt_iter_lock);
}

void
mount_iterate_unlock(mount_t mp)
{
	lck_mtx_unlock(&mp->mnt_iter_lock);
}

		if (vfsp->vfc_mountroot == NULL
			&& !ISSET(vfsp->vfc_vfsflags, VFC_VFSCANMOUNTROOT)) {
		}
		if (vfsp->vfc_mountroot)
			error = (*vfsp->vfc_mountroot)(mp, rootvp, ctx);
		else
			error = VFS_MOUNT(mp, rootvp, 0, ctx);

		if (!error) {
			if (mp->mnt_ioflags & MNT_IOFLAGS_FUSION_DRIVE) {

#if !CONFIG_EMBEDDED
			if (MNTK_VIRTUALDEV & mp->mnt_kern_flag)    speed = 128;
			else if (disk_conditioner_mount_is_ssd(mp)) speed = 7*256;
			else                                        speed = 256;
#endif

				if ((vfsattr.f_capabilities.capabilities[VOL_CAPABILITIES_FORMAT] & VOL_CAP_FMT_DIR_HARDLINKS) &&
					(vfsattr.f_capabilities.valid[VOL_CAPABILITIES_FORMAT] & VOL_CAP_FMT_DIR_HARDLINKS)) {
					mp->mnt_kern_flag |= MNTK_DIR_HARDLINKS;
				}
	while (vfs_getvfs_locked(&tfsid)) {
		if (++mntid_gen == 0)
			mntid_gen++;
		tfsid.val[0] = makedev(nblkdev + mtype, mntid_gen);

	/*
	 * See comments in vnode_iterate() for the rationale for this lock
	 */
	mount_iterate_lock(mp);

			mount_iterate_unlock(mp);
	/* If it returns 0 then there is nothing to do */
		mount_iterate_unlock(mp);
	mount_iterate_unlock(mp);
	if (!vnode_isspec(vp)) {
		return (vp->v_usecount - vp->v_kusecount);		
	}

/*
 * vn_getpath_fsenter_with_parent will reenter the file system to fine the path of the
 * vnode.  It requires that there are IO counts on both the vnode and the directory vnode.
 *
 * vn_getpath_fsenter is called by MAC hooks to authorize operations for every thing, but
 * unlink, rmdir and rename. For these operation the MAC hook  calls vn_getpath. This presents
 * problems where if the path can not be found from the name cache, those operations can
 * erroneously fail with EPERM even though the call should succeed. When removing or moving
 * file system objects with operations such as unlink or rename, those operations need to
 * take IO counts on the target and containing directory. Calling vn_getpath_fsenter from a
 * MAC hook from these operations during forced unmount operations can lead to dead
 * lock. This happens when the operation starts, IO counts are taken on the containing
 * directories and targets. Before the MAC hook is called a forced unmount from another
 * thread takes place and blocks on the on going operation's directory vnode in vdrain.
 * After which, the MAC hook gets called and calls vn_getpath_fsenter.  vn_getpath_fsenter
 * is called with the understanding that there is an IO count on the target. If in
 * build_path the directory vnode is no longer in the cache, then the parent object id via
 * vnode_getattr from the target is obtain and used to call VFS_VGET to get the parent
 * vnode. The file system's VFS_VGET then looks up by inode in its hash and tries to get
 * an IO count. But VFS_VGET "sees" the directory vnode is in vdrain and can block
 * depending on which version and how it calls the vnode_get family of interfaces.
 *
 * N.B.  A reasonable interface to use is vnode_getwithvid. This interface was modified to
 * call vnode_getiocount with VNODE_DRAINO, so it will happily get an IO count and not
 * cause issues, but there is no guarantee that all or any file systems are doing that.
 *
 * vn_getpath_fsenter_with_parent can enter the file system safely since there is a known
 * IO count on the directory vnode by calling build_path_with_parent.
 */

int
vn_getpath_fsenter_with_parent(struct vnode *dvp, struct vnode *vp, char *pathbuf, int *len)
{
	return build_path_with_parent(vp, dvp, pathbuf, *len, len, 0, vfs_context_current());
}

int is_package_name(const char *name, int len)
	u_int32_t minsaturationbytecount = 0;
	
	if (VNOP_IOCTL(devvp, DKIOCGETIOMINSATURATIONBYTECOUNT, (caddr_t)&minsaturationbytecount, 0, ctx) == 0) {
		mp->mnt_minsaturationbytecount = minsaturationbytecount;
	} else {
		mp->mnt_minsaturationbytecount = 0;
	}
	} else {
		/* Check for APFS Fusion */
		dk_apfs_flavour_t flavour;
		if ((VNOP_IOCTL(devvp, DKIOCGETAPFSFLAVOUR, (caddr_t)&flavour, 0, ctx) == 0) &&
		    (flavour == DK_APFS_FUSION)) {
			mp->mnt_ioflags |= MNT_IOFLAGS_FUSION_DRIVE;
		}
#ifdef NFSCLIENT
				strlcpy(&sfs.f_fstypename[0], &mp->fstypename_override[0], MFSNAMELEN);
			} else
#endif
			{

#ifdef NFSCLIENT
				strlcpy(&sfs.f_fstypename[0], &mp->fstypename_override[0], MFSNAMELEN);
			} else
#endif
			{
static int	filt_fsattach(struct knote *kn, struct kevent_internal_s *kev);
static int	filt_fstouch(struct knote *kn, struct kevent_internal_s *kev);
static int	filt_fsprocess(struct knote *kn, struct filt_process_s *data, struct kevent_internal_s *kev);
SECURITY_READ_ONLY_EARLY(struct filterops) fs_filtops = {
	.f_attach = filt_fsattach,
	.f_detach = filt_fsdetach,
	.f_event = filt_fsevent,
	.f_touch = filt_fstouch,
	.f_process = filt_fsprocess,
filt_fsattach(struct knote *kn, __unused struct kevent_internal_s *kev)

	/* 
	 * filter only sees future events, 
	 * so it can't be fired already.
	 */
static int
filt_fstouch(struct knote *kn, struct kevent_internal_s *kev)
{
	int res;

	lck_mtx_lock(fs_klist_lock);

	kn->kn_sfflags = kev->fflags;
	if ((kn->kn_status & KN_UDATA_SPECIFIC) == 0)
		kn->kn_udata = kev->udata;

	/*
	 * the above filter function sets bits even if nobody is looking for them.
	 * Just preserve those bits even in the new mask is more selective
	 * than before.
	 *
	 * For compatibility with previous implementations, we leave kn_fflags
	 * as they were before.
	 */
	//if (kn->kn_sfflags)
	//	kn->kn_fflags &= kn->kn_sfflags;
	res = (kn->kn_fflags != 0);

	lck_mtx_unlock(fs_klist_lock);

	return res;
}

static int
filt_fsprocess(struct knote *kn, struct filt_process_s *data, struct kevent_internal_s *kev)
{
#pragma unused(data)
	int res;

	lck_mtx_lock(fs_klist_lock);
	res = (kn->kn_fflags != 0);
	if (res) {
		*kev = kn->kn_kevent;
		kn->kn_flags |= EV_CLEAR; /* automatic */
		kn->kn_fflags = 0;
		kn->kn_data = 0;
	}
	lck_mtx_unlock(fs_klist_lock);
	return res;
}	

/* Indicate that the root file system unmounted cleanly */
static int vfs_root_unmounted_cleanly = 0;
SYSCTL_INT(_vfs_generic, OID_AUTO, root_unmounted_cleanly, CTLFLAG_RD, &vfs_root_unmounted_cleanly, 0, "Root filesystem was unmounted cleanly");

void
vfs_set_root_unmounted_cleanly(void)
{
	vfs_root_unmounted_cleanly = 1;
}

			if ( !LIST_EMPTY(&vp->v_nclinks) || !TAILQ_EMPTY(&vp->v_ncchildren))
__attribute__((noreturn))
		TAILQ_INIT(&vp->v_ncchildren);

int vnode_usecount(vnode_t vp)
{
	return vp->v_usecount;
}

int vnode_iocount(vnode_t vp)
{
	return vp->v_iocount;
}

#if CONFIG_SECLUDED_MEMORY
	switch (secluded_for_filecache) {
	case 0:
		/*
		 * secluded_for_filecache == 0:
		 * + no file contents in secluded pool
		 */
		break;
	case 1:
		/*
		 * secluded_for_filecache == 1:
		 * + no files from /
		 * + files from /Applications/ are OK
		 * + files from /Applications/Camera are not OK
		 * + no files that are open for write
		 */
		if (vnode_vtype(vp) == VREG &&
		    vnode_mount(vp) != NULL &&
		    (! (vfs_flags(vnode_mount(vp)) & MNT_ROOTFS))) {
			/* not from root filesystem: eligible for secluded pages */
			memory_object_mark_eligible_for_secluded(
				ubc_getobject(vp, UBC_FLAGS_NONE),
				TRUE);
		}
		break;
	case 2:
		/*
		 * secluded_for_filecache == 2:
		 * + all read-only files OK, except:
		 * 	+ dyld_shared_cache_arm64*
		 * 	+ Camera
		 *	+ mediaserverd
		 */
		if (vnode_vtype(vp) == VREG) {
			memory_object_mark_eligible_for_secluded(
				ubc_getobject(vp, UBC_FLAGS_NONE),
				TRUE);
		}
		break;
	default:
		break;
	}
#endif /* CONFIG_SECLUDED_MEMORY */


	if ((error = vfs_getattr(mp, &va, ctx)) != 0) {
		KAUTH_DEBUG("STAT - filesystem returned error %d", error);
		return(error);
	}
static int vnode_authorize_callback_int(kauth_action_t action, vfs_context_t ctx,
    vnode_t vp, vnode_t dvp, int *errorp);
#define _VAC_NO_VNODE_POINTERS	(1<<4)
			!(vp->v_mount->mnt_kern_flag & MNTK_DIR_HARDLINKS)) {
int
					struct vnode *tdvp,  struct vnode *tvp,  struct componentname *tcnp,
					vfs_context_t ctx, void *reserved)
{
	return vn_authorize_renamex(fdvp, fvp, fcnp, tdvp, tvp, tcnp, ctx, 0, reserved);
}

int
vn_authorize_renamex(struct vnode *fdvp,  struct vnode *fvp,  struct componentname *fcnp,
					 struct vnode *tdvp,  struct vnode *tvp,  struct componentname *tcnp,
					 vfs_context_t ctx, vfs_rename_flags_t flags, void *reserved)
	bool swap = flags & VFS_RENAME_SWAP;
	if (swap) {
		error = mac_vnode_check_rename(ctx, tdvp, tvp, tcnp, fdvp, fvp, fcnp);
		if (error)
			goto out;
	}
		if (!swap) {
			if (fvp->v_type == VDIR && tvp->v_type != VDIR) {
				error = ENOTDIR;
				goto out;
			} else if (fvp->v_type != VDIR && tvp->v_type == VDIR) {
				error = EISDIR;
				goto out;
			}
	} else if (swap) {
		/*
		 * Caller should have already checked this and returned
		 * ENOENT.  If we send back ENOENT here, caller will retry
		 * which isn't what we want so we send back EINVAL here
		 * instead.
		 */
		error = EINVAL;
		goto out;

	if (swap && fdvp->v_parent == tvp) {
		error = EINVAL;
		goto out;
	}
	if (swap) {
		kauth_action_t f = 0, t = 0;
		/*
		 * Directories changing parents need ...ADD_SUBDIR...  to
		 * permit changing ".."
		 */
		if (fdvp != tdvp) {
			if (vnode_isdir(fvp))
				f = KAUTH_VNODE_ADD_SUBDIRECTORY;
			if (vnode_isdir(tvp))
				t = KAUTH_VNODE_ADD_SUBDIRECTORY;
		}
		error = vnode_authorize(fvp, fdvp, KAUTH_VNODE_DELETE | f, ctx);
		if (error)
		error = vnode_authorize(tvp, tdvp, KAUTH_VNODE_DELETE | t, ctx);
		if (error)
		f = vnode_isdir(fvp) ? KAUTH_VNODE_ADD_SUBDIRECTORY : KAUTH_VNODE_ADD_FILE;
		t = vnode_isdir(tvp) ? KAUTH_VNODE_ADD_SUBDIRECTORY : KAUTH_VNODE_ADD_FILE;
		if (fdvp == tdvp)
			error = vnode_authorize(fdvp, NULL, f | t, ctx);
		else {
			error = vnode_authorize(fdvp, NULL, t, ctx);
			if (error)
				goto out;
			error = vnode_authorize(tdvp, NULL, f, ctx);
		if (error)
			goto out;
		error = 0;
		if ((tvp != NULL) && vnode_isdir(tvp)) {
			if (tvp != fdvp)
				moving = 1;
		} else if (tdvp != fdvp) {
			moving = 1;
		}

		/*
		 * must have delete rights to remove the old name even in
		 * the simple case of fdvp == tdvp.
		 *
		 * If fvp is a directory, and we are changing it's parent,
		 * then we also need rights to rewrite its ".." entry as well.
		 */
		if (vnode_isdir(fvp)) {
			if ((error = vnode_authorize(fvp, fdvp, KAUTH_VNODE_DELETE | KAUTH_VNODE_ADD_SUBDIRECTORY, ctx)) != 0)
				goto out;
		} else {
			if ((error = vnode_authorize(fvp, fdvp, KAUTH_VNODE_DELETE, ctx)) != 0)
				goto out;
		}
		if (moving) {
			/* moving into tdvp or tvp, must have rights to add */
			if ((error = vnode_authorize(((tvp != NULL) && vnode_isdir(tvp)) ? tvp : tdvp,
							NULL, 
							vnode_isdir(fvp) ? KAUTH_VNODE_ADD_SUBDIRECTORY : KAUTH_VNODE_ADD_FILE,
							ctx)) != 0) {
				goto out;
			}
		} else {
			/* node staying in same directory, must be allowed to add new name */
			if ((error = vnode_authorize(fdvp, NULL,
							vnode_isdir(fvp) ? KAUTH_VNODE_ADD_SUBDIRECTORY : KAUTH_VNODE_ADD_FILE, ctx)) != 0)
				goto out;
		}
		/* overwriting tvp */
		if ((tvp != NULL) && !vnode_isdir(tvp) &&
				((error = vnode_authorize(tvp, tdvp, KAUTH_VNODE_DELETE, ctx)) != 0)) {
		}
 * Authorizer for directory cloning. This does not use vnodes but instead
 * uses prefilled vnode attributes from the filesystem.
 * The same function is called to set up the attributes required, perform the
 * authorization and cleanup (if required)
vnode_attr_authorize_dir_clone(struct vnode_attr *vap, kauth_action_t action,
    struct vnode_attr *dvap, __unused vnode_t sdvp, mount_t mp,
    dir_clone_authorizer_op_t vattr_op, uint32_t flags, vfs_context_t ctx,
    __unused void *reserved)
	int error;
	int is_suser = vfs_context_issuser(ctx);
	if (vattr_op == OP_VATTR_SETUP) {
		VATTR_INIT(vap);

		/*
		 * When ACL inheritence is implemented, both vap->va_acl and
		 * dvap->va_acl will be required (even as superuser).
		 */
		VATTR_WANTED(vap, va_type);
		VATTR_WANTED(vap, va_mode);
		VATTR_WANTED(vap, va_flags);
		VATTR_WANTED(vap, va_uid);
		VATTR_WANTED(vap, va_gid);
		if (dvap) {
			VATTR_INIT(dvap);
			VATTR_WANTED(dvap, va_flags);
		}

		if (!is_suser) {
			/*
			 * If not superuser, we have to evaluate ACLs and
			 * need the target directory gid to set the initial
			 * gid of the new object.
			 */
			VATTR_WANTED(vap, va_acl);
			if (dvap)
				VATTR_WANTED(dvap, va_gid);
		} else if (dvap && (flags & VNODE_CLONEFILE_NOOWNERCOPY)) {
			VATTR_WANTED(dvap, va_gid);
		}
		return (0);
	} else if (vattr_op == OP_VATTR_CLEANUP) {
		return (0); /* Nothing to do for now */
	}

	/* dvap isn't used for authorization */
	error = vnode_attr_authorize(vap, NULL, mp, action, ctx);

	if (error)
		return (error);

	/*
	 * vn_attribute_prepare should be able to accept attributes as well as
	 * vnodes but for now we do this inline.
	 */
	if (!is_suser || (flags & VNODE_CLONEFILE_NOOWNERCOPY)) {
		/*
		 * If the filesystem is mounted IGNORE_OWNERSHIP and an explicit
		 * owner is set, that owner takes ownership of all new files.
		 */
		if ((mp->mnt_flag & MNT_IGNORE_OWNERSHIP) &&
		    (mp->mnt_fsowner != KAUTH_UID_NONE)) {
			VATTR_SET(vap, va_uid, mp->mnt_fsowner);
		} else {
			/* default owner is current user */
			VATTR_SET(vap, va_uid,
			    kauth_cred_getuid(vfs_context_ucred(ctx)));
		}

		if ((mp->mnt_flag & MNT_IGNORE_OWNERSHIP) &&
		    (mp->mnt_fsgroup != KAUTH_GID_NONE)) {
			VATTR_SET(vap, va_gid, mp->mnt_fsgroup);
		} else {
			/*
			 * default group comes from parent object,
			 * fallback to current user
			 */
			if (VATTR_IS_SUPPORTED(dvap, va_gid)) {
				VATTR_SET(vap, va_gid, dvap->va_gid);
			} else {
				VATTR_SET(vap, va_gid,
				    kauth_cred_getgid(vfs_context_ucred(ctx)));
			}
		}
	}

	/* Inherit SF_RESTRICTED bit from destination directory only */
	if (VATTR_IS_ACTIVE(vap, va_flags)) {
		VATTR_SET(vap, va_flags,
		    ((vap->va_flags & ~(UF_DATAVAULT | SF_RESTRICTED)))); /* Turn off from source */
		 if (VATTR_IS_ACTIVE(dvap, va_flags))
			VATTR_SET(vap, va_flags,
			    vap->va_flags | (dvap->va_flags & (UF_DATAVAULT | SF_RESTRICTED)));
	} else if (VATTR_IS_ACTIVE(dvap, va_flags)) {
		VATTR_SET(vap, va_flags, (dvap->va_flags & (UF_DATAVAULT | SF_RESTRICTED)));
	}

	return (0);
}


/*
 * Authorize an operation on a vnode.
 *
 * This is KPI, but here because it needs vnode_scope.
 *
 * Returns:	0			Success
 *	kauth_authorize_action:EPERM	...
 *	xlate => EACCES			Permission denied
 *	kauth_authorize_action:0	Success
 *	kauth_authorize_action:		Depends on callback return; this is
 *					usually only vnode_authorize_callback(),
 *					but may include other listerners, if any
 *					exist.
 *		EROFS
 *		EACCES
 *		EPERM
 *		???
 */
int
vnode_authorize(vnode_t vp, vnode_t dvp, kauth_action_t action, vfs_context_t ctx)
{
	int	error, result;

	/*
	 * We can't authorize against a dead vnode; allow all operations through so that
	 * the correct error can be returned.
	 */
	if (vp->v_type == VBAD)
		return(0);
	
	error = 0;
	result = kauth_authorize_action(vnode_scope, vfs_context_ucred(ctx), action,
		   (uintptr_t)ctx, (uintptr_t)vp, (uintptr_t)dvp, (uintptr_t)&error);
	if (result == EPERM)		/* traditional behaviour */
		result = EACCES;
	/* did the lower layers give a better error return? */
 * The precedence of factors for authorizing or denying delete for a credential
 *
 * 1) Explicit ACE on the node. (allow or deny DELETE)
 * 2) Explicit ACE on the directory (allow or deny DELETE_CHILD).
 *    If there are conflicting ACEs on the node and the directory, the node
 *    ACE wins.
 *
 * 3) Sticky bit on the directory.
 *    Deletion is not permitted if the directory is sticky and the caller is
 *    not owner of the node or directory. The sticky bit rules are like a deny
 *    delete ACE except lower in priority than ACL's either allowing or denying
 *    delete.
 *
 * 4) POSIX permisions on the directory.
 * on directories. This enables us to skip directory ACL and POSIX checks
 * as we already have the result from those checks. However, we always check the
 * node ACL and, if the directory has the sticky bit set, we always check its
 * ACL (even for a directory with an authorized delete child). Furthermore,
 * caching the delete child authorization is independent of the sticky bit
 * being set as it is only applicable in determining whether the node can be
 * deleted or not.
static int
	int			error, ismember;
	/* Check the ACL on the node first */
	if (VATTR_IS_NOT(vap, va_acl, NULL)) {
		eval.ae_requested = KAUTH_VNODE_DELETE;
		eval.ae_acl = &vap->va_acl->acl_ace[0];
		eval.ae_count = vap->va_acl->acl_entrycount;
		if (vauth_file_owner(vcp))
		if ((error = vauth_file_ingroup(vcp, &ismember, ENOENT)) != 0 && error != ENOENT)
			return (error);
		if ((error = kauth_acl_evaluate(cred, &eval)) != 0) {
			return (error);

			KAUTH_DEBUG("%p    DENIED - denied by ACL", vcp->vp);
			return (EACCES);
		case KAUTH_RESULT_ALLOW:
			KAUTH_DEBUG("%p    ALLOWED - granted by ACL", vcp->vp);
			return (0);
			/* Defer to directory */
			KAUTH_DEBUG("%p    DEFERRED - by file ACL", vcp->vp);
	/*
	 * Without a sticky bit, a previously authorized delete child is
	 * sufficient to authorize this delete.
	 *
	 * If the sticky bit is set, a directory ACL which allows delete child
	 * overrides a (potential) sticky bit deny. The authorized delete child
	 * cannot tell us if it was authorized because of an explicit delete
	 * child allow ACE or because of POSIX permisions so we have to check
	 * the directory ACL everytime if the directory has a sticky bit.
	 */
	if (!(dvap->va_mode & S_ISTXT) && cached_delete_child) {
		KAUTH_DEBUG("%p    ALLOWED - granted by directory ACL or POSIX permissions and no sticky bit on directory", vcp->vp);
		return (0);
	}

	/* check the ACL on the directory */
	if (VATTR_IS_NOT(dvap, va_acl, NULL)) {
		eval.ae_requested = KAUTH_VNODE_DELETE_CHILD;
		eval.ae_acl = &dvap->va_acl->acl_ace[0];
		eval.ae_count = dvap->va_acl->acl_entrycount;
		if (vauth_dir_owner(vcp))
		if ((error = vauth_dir_ingroup(vcp, &ismember, ENOENT)) != 0 && error != ENOENT)
		/*
		 * If there is no entry, we are going to defer to other
		 * authorization mechanisms.
		 */
		error = kauth_acl_evaluate(cred, &eval);

		if (error != 0) {
			return (error);
			KAUTH_DEBUG("%p    DENIED - denied by directory ACL", vcp->vp);
			return (EACCES);
			KAUTH_DEBUG("%p    ALLOWED - granted by directory ACL", vcp->vp);
			if (!cached_delete_child && vcp->dvp) {
				vnode_cache_authorized_action(vcp->dvp,
				    vcp->ctx, KAUTH_VNODE_DELETE_CHILD);
			}
			return (0);
			/* Deferred by directory ACL */
			KAUTH_DEBUG("%p    DEFERRED - directory ACL", vcp->vp);
	/*
	 * From this point, we can't explicitly allow and if we reach the end
	 * of the function without a denial, then the delete is authorized.
	 */
	if (!cached_delete_child) {
		if (vnode_authorize_posix(vcp, VWRITE, 1 /* on_dir */) != 0) {
			KAUTH_DEBUG("%p    DENIED - denied by posix permisssions", vcp->vp);
			return (EACCES);
		}
		/*
		 * Cache the authorized action on the vnode if allowed by the
		 * directory ACL or POSIX permissions. It is correct to cache
		 * this action even if sticky bit would deny deleting the node.
		 */
		if (vcp->dvp) {
			vnode_cache_authorized_action(vcp->dvp, vcp->ctx,
			    KAUTH_VNODE_DELETE_CHILD);
		}
		return (EACCES);
	return (0);
vnode_authorize_checkimmutable(mount_t mp, struct vnode_attr *vap, int rights, int ignore)
	switch(vap->va_type) {
		if (vap->va_type == VDIR) {
vnode_authorize_callback(__unused kauth_cred_t cred, __unused void *idata,
    kauth_action_t action, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2,
    uintptr_t arg3)
        result = vnode_authorize_callback_int(action, ctx, vp, dvp, (int *)arg3);
static int
vnode_attr_authorize_internal(vauth_ctx vcp, mount_t mp,
    kauth_ace_rights_t rights, int is_suser, boolean_t *found_deny,
    int noimmutable, int parent_authorized_for_delete_child)
{
	int result;

	/*
	 * Check for immutability.
	 *
	 * In the deletion case, parent directory immutability vetoes specific
	 * file rights.
	 */
	if ((result = vnode_authorize_checkimmutable(mp, vcp->vap, rights,
	    noimmutable)) != 0)
		goto out;

	if ((rights & KAUTH_VNODE_DELETE) &&
	    !parent_authorized_for_delete_child) {
		result = vnode_authorize_checkimmutable(mp, vcp->dvap,
		    KAUTH_VNODE_DELETE_CHILD, 0);
		if (result)
			goto out;
	}

	/*
	 * Clear rights that have been authorized by reaching this point, bail if nothing left to
	 * check.
	 */
	rights &= ~(KAUTH_VNODE_LINKTARGET | KAUTH_VNODE_CHECKIMMUTABLE);
	if (rights == 0)
		goto out;

	/*
	 * If we're not the superuser, authorize based on file properties;
	 * note that even if parent_authorized_for_delete_child is TRUE, we
	 * need to check on the node itself.
	 */
	if (!is_suser) {
		/* process delete rights */
		if ((rights & KAUTH_VNODE_DELETE) &&
		    ((result = vnode_authorize_delete(vcp, parent_authorized_for_delete_child)) != 0))
		    goto out;

		/* process remaining rights */
		if ((rights & ~KAUTH_VNODE_DELETE) &&
		    (result = vnode_authorize_simple(vcp, rights, rights & KAUTH_VNODE_DELETE, found_deny)) != 0)
			goto out;
	} else {
		/*
		 * Execute is only granted to root if one of the x bits is set.  This check only
		 * makes sense if the posix mode bits are actually supported.
		 */
		if ((rights & KAUTH_VNODE_EXECUTE) &&
		    (vcp->vap->va_type == VREG) &&
		    VATTR_IS_SUPPORTED(vcp->vap, va_mode) &&
		    !(vcp->vap->va_mode & (S_IXUSR | S_IXGRP | S_IXOTH))) {
			result = EPERM;
			KAUTH_DEBUG("%p    DENIED - root execute requires at least one x bit in 0x%x", vp, va.va_mode);
			goto out;
		}

		/* Assume that there were DENYs so we don't wrongly cache KAUTH_VNODE_SEARCHBYANYONE */
		*found_deny = TRUE;

		KAUTH_DEBUG("%p    ALLOWED - caller is superuser", vp);
	}
out:
	return (result);
}
vnode_authorize_callback_int(kauth_action_t action, vfs_context_t ctx,
    vnode_t vp, vnode_t dvp, int *errorp)
	boolean_t		is_suser = FALSE;
	vcp->ctx = ctx;
	vcp->vp = vp;
	vcp->dvp = dvp;
		vcp->dvp = NULLVP;
		vcp->dvap = NULL;
	 * If the vnode is a namedstream (extended attribute) data vnode (eg.
	 * a resource fork), *_DATA becomes *_EXTATTRIBUTES.

		/*
		 * Point 'vp' to the namedstream's parent for ACL checking
		 */
		if ((vp->v_parent != NULL) &&
		    (vget_internal(vp->v_parent, 0, VNODE_NODEAD | VNODE_DRAINO) == 0)) {
			parent_ref = TRUE;
			vcp->vp = vp = vp->v_parent;
		}
	}

	if (vfs_context_issuser(ctx)) {
		/*
		 * if we're not asking for execute permissions or modifications,
		 * then we're done, this action is authorized.
		 */
		if (!(rights & (KAUTH_VNODE_EXECUTE | KAUTH_VNODE_WRITE_RIGHTS)))
			goto success;

		is_suser = TRUE;
	 * Get vnode attributes and extended security information for the vnode
	 * and directory if required.
	 *
	 * If we're root we only want mode bits and flags for checking
	 * execute and immutability.
	VATTR_WANTED(&va, va_mode);
	VATTR_WANTED(&va, va_flags);
	if (!is_suser) {
	if ((result = vnode_getattr(vp, &va, ctx)) != 0) {
		KAUTH_DEBUG("%p    ERROR - failed to get vnode attributes - %d", vp, result);
	}
	VATTR_WANTED(&va, va_type);
	VATTR_RETURN(&va, va_type, vnode_vtype(vp));
	if (vcp->dvp) {
		VATTR_WANTED(&dva, va_mode);
		VATTR_WANTED(&dva, va_flags);
		if (!is_suser) {
			VATTR_WANTED(&dva, va_uid);
			VATTR_WANTED(&dva, va_gid);
			VATTR_WANTED(&dva, va_acl);
		}
		if ((result = vnode_getattr(vcp->dvp, &dva, ctx)) != 0) {
			KAUTH_DEBUG("%p    ERROR - failed to get directory vnode attributes - %d", vp, result);
		VATTR_WANTED(&dva, va_type);
		VATTR_RETURN(&dva, va_type, vnode_vtype(vcp->dvp));

	result = vnode_attr_authorize_internal(vcp, vp->v_mount, rights, is_suser,
	    &found_deny, noimmutable, parent_authorized_for_delete_child);
		 *
		 * Note that we can correctly cache KAUTH_VNODE_SEARCHBYANYONE
		 * only if we actually check ACLs which we don't for root. As
		 * a workaround, the lookup fast path checks for root.
success:

int
vnode_attr_authorize_init(struct vnode_attr *vap, struct vnode_attr *dvap,
    kauth_action_t action, vfs_context_t ctx)
{
	VATTR_INIT(vap);
	VATTR_WANTED(vap, va_type);
	VATTR_WANTED(vap, va_mode);
	VATTR_WANTED(vap, va_flags);
	if (dvap) {
		VATTR_INIT(dvap);
		if (action & KAUTH_VNODE_DELETE) {
			VATTR_WANTED(dvap, va_type);
			VATTR_WANTED(dvap, va_mode);
			VATTR_WANTED(dvap, va_flags);
		}
	} else if (action & KAUTH_VNODE_DELETE) {
		return (EINVAL);
	}

	if (!vfs_context_issuser(ctx)) {
		VATTR_WANTED(vap, va_uid);
		VATTR_WANTED(vap, va_gid);
		VATTR_WANTED(vap, va_acl);
		if (dvap && (action & KAUTH_VNODE_DELETE)) {
			VATTR_WANTED(dvap, va_uid);
			VATTR_WANTED(dvap, va_gid);
			VATTR_WANTED(dvap, va_acl);
		}
	}

	return (0);
}

int
vnode_attr_authorize(struct vnode_attr *vap, struct vnode_attr *dvap, mount_t mp,
    kauth_action_t action, vfs_context_t ctx)
{
	struct _vnode_authorize_context auth_context;
	vauth_ctx vcp;
	kauth_ace_rights_t rights;
	int noimmutable;
	boolean_t found_deny;
	boolean_t is_suser = FALSE;
	int result = 0;

	vcp = &auth_context;
	vcp->ctx = ctx;
	vcp->vp = NULLVP;
	vcp->vap = vap;
	vcp->dvp = NULLVP;
	vcp->dvap = dvap;
	vcp->flags = vcp->flags_valid = 0;

	noimmutable = (action & KAUTH_VNODE_NOIMMUTABLE) ? 1 : 0;
	rights = action & ~(KAUTH_VNODE_ACCESS | KAUTH_VNODE_NOIMMUTABLE);

	/*
	 * Check for read-only filesystems.
	 */
	if ((rights & KAUTH_VNODE_WRITE_RIGHTS) &&
	    mp && (mp->mnt_flag & MNT_RDONLY) &&
	    ((vap->va_type == VREG) || (vap->va_type == VDIR) ||
	    (vap->va_type == VLNK) || (rights & KAUTH_VNODE_DELETE) ||
	    (rights & KAUTH_VNODE_DELETE_CHILD))) {
		result = EROFS;
		goto out;
	}

	/*
	 * Check for noexec filesystems.
	 */
	if ((rights & KAUTH_VNODE_EXECUTE) &&
	    (vap->va_type == VREG) && mp && (mp->mnt_flag & MNT_NOEXEC)) {
		result = EACCES;
		goto out;
	}

	if (vfs_context_issuser(ctx)) {
		/*
		 * if we're not asking for execute permissions or modifications,
		 * then we're done, this action is authorized.
		 */
		if (!(rights & (KAUTH_VNODE_EXECUTE | KAUTH_VNODE_WRITE_RIGHTS)))
			goto out;
		is_suser = TRUE;
	} else {
		if (!VATTR_IS_SUPPORTED(vap, va_uid) ||
		    !VATTR_IS_SUPPORTED(vap, va_gid) ||
		    (mp && vfs_extendedsecurity(mp) && !VATTR_IS_SUPPORTED(vap, va_acl))) {
			panic("vnode attrs not complete for vnode_attr_authorize\n");
		}
	}

	result = vnode_attr_authorize_internal(vcp, mp, rights, is_suser,
	    &found_deny, noimmutable, FALSE);

	if (result == EPERM)
		result = EACCES;
out:
	return (result);
}


	uint32_t	inherit_flags;
	struct vnode_attr dva;
	inherit_flags = 0;

	/*
	 * We need the dvp's va_flags and *may* need the gid of the directory,
	 * we ask for both here.
	 */
	VATTR_INIT(&dva);
	VATTR_WANTED(&dva, va_gid);
	VATTR_WANTED(&dva, va_flags);
	if ((error = vnode_getattr(dvp, &dva, ctx)) != 0)
		goto out;


	/* Determine if SF_RESTRICTED should be inherited from the parent
	 * directory. */
	if (VATTR_IS_SUPPORTED(&dva, va_flags)) {
		inherit_flags = dva.va_flags & (UF_DATAVAULT | SF_RESTRICTED);
	}

	if (inherit_flags) {
		/* Apply SF_RESTRICTED to the file if its parent directory was
		 * restricted.  This is done at the end so that root is not
		 * required if this flag is only set due to inheritance. */
		VATTR_SET(vap, va_flags, (vap->va_flags | inherit_flags));
	}
	    VATTR_IS_ACTIVE(vap, va_backup_time) ||
	    VATTR_IS_ACTIVE(vap, va_addedtime)) {
	    VATTR_IS_ACTIVE(vap, va_backup_time) ||
	    VATTR_IS_ACTIVE(vap, va_addedtime)) {
		}
			
		/*
		 * clear set-uid and set-gid bits. POSIX only requires this for
		 * non-privileged processes but we do it even for root.
		 */
		if (VATTR_IS_ACTIVE(vap, va_mode)) {
			newmode = vap->va_mode;
		} else if (VATTR_IS_SUPPORTED(&ova, va_mode)) {
			newmode = ova.va_mode;
		} else {
			KAUTH_DEBUG("CHOWN - trying to change owner but cannot get mode from filesystem to mask setugid bits");
			newmode = 0;
		}

		/* chown always clears setuid/gid bits. An exception is made for
		 * setattrlist executed by a root process to set <uid, gid, mode> on a file:
		 * setattrlist is allowed to set the new mode on the file and change (chown)
		 * uid/gid.
		 */
		if (newmode & (S_ISUID | S_ISGID)) {
			if (!VATTR_IS_ACTIVE(vap, va_mode) || !has_priv_suser) {
				KAUTH_DEBUG("CHOWN - masking setugid bits from mode %o to %o",
					newmode, newmode & ~(S_ISUID | S_ISGID));
				newmode &= ~(S_ISUID | S_ISGID);
			VATTR_SET(vap, va_mode, newmode);

	paniclog_append_noflush("\n***** VNODES *****\n"

		if (!ml_validate_nofault((vm_offset_t)mnt, sizeof(mount_t))) {
			paniclog_append_noflush("Unable to iterate the mount list %p - encountered an invalid mount pointer %p \n",
				&mountlist, mnt);
			break;
		}


			if (!ml_validate_nofault((vm_offset_t)vp, sizeof(vnode_t))) {
				paniclog_append_noflush("Unable to iterate the vnode list %p - encountered an invalid vnode pointer %p \n",
					&mnt->mnt_vnodelist, vp);
				break;
			}
				
			paniclog_append_noflush("%s %0d %0d %s\n",

vm_offset_t kdebug_vnode(vnode_t vp)
{
	return VM_KERNEL_ADDRPERM(vp);
}

static int flush_cache_on_write = 0;
SYSCTL_INT (_kern, OID_AUTO, flush_cache_on_write,
			CTLFLAG_RW | CTLFLAG_LOCKED, &flush_cache_on_write, 0,
			"always flush the drive cache on writes to uncached files");

int vnode_should_flush_after_write(vnode_t vp, int ioflag)
{
	return (flush_cache_on_write
			&& (ISSET(ioflag, IO_NOCACHE) || vnode_isnocache(vp)));
}

/*
 * sysctl for use by disk I/O tracing tools to get the list of existing
 * vnodes' paths
 */

struct vnode_trace_paths_context {
	uint64_t count;
	long path[MAXPATHLEN / sizeof (long) + 1]; /* + 1 in case sizeof (long) does not divide MAXPATHLEN */
};

static int vnode_trace_path_callback(struct vnode *vp, void *arg) {
	int len, rv;
	struct vnode_trace_paths_context *ctx;

	ctx = arg;

	len = sizeof (ctx->path);
	rv = vn_getpath(vp, (char *)ctx->path, &len);
	/* vn_getpath() NUL-terminates, and len includes the NUL */

	if (!rv) {
		kdebug_lookup_gen_events(ctx->path, len, vp, TRUE);

		if (++(ctx->count) == 1000) {
			thread_yield_to_preemption();
			ctx->count = 0;
		}
	}

	return VNODE_RETURNED;
}

static int vfs_trace_paths_callback(mount_t mp, void *arg) {
	if (mp->mnt_flag & MNT_LOCAL)
		vnode_iterate(mp, VNODE_ITERATE_ALL, vnode_trace_path_callback, arg);

	return VFS_RETURNED;
}

static int sysctl_vfs_trace_paths SYSCTL_HANDLER_ARGS {
	struct vnode_trace_paths_context ctx;

	(void)oidp;
	(void)arg1;
	(void)arg2;
	(void)req;

	if (!kauth_cred_issuser(kauth_cred_get()))
		return EPERM;

	if (!kdebug_enable || !kdebug_debugid_enabled(VFS_LOOKUP))
		return EINVAL;

	bzero(&ctx, sizeof (struct vnode_trace_paths_context));

	vfs_iterate(0, vfs_trace_paths_callback, &ctx);

	return 0;
}

SYSCTL_PROC(_vfs_generic, OID_AUTO, trace_paths, CTLFLAG_RD | CTLFLAG_LOCKED | CTLFLAG_MASKED, NULL, 0, &sysctl_vfs_trace_paths, "-", "trace_paths");