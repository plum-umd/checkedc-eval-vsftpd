/*
 * Part of Very Secure FTPd
 * Licence: GPL v2
 * Author: Chris Evans
 * sysstr.c
 *
 * This file basically wraps system functions so that we can deal in our
 * nice abstracted string buffer objects.
 */

#include "sysstr.h"
#include "str.h"
#include "secbuf.h"
#include "sysutil.h"
#include "defs.h"
#include "utility.h"
#include "tunables.h"

void
str_getcwd(_Ptr<struct mystr> p_str)
{
  static char* p_getcwd_buf;
  _Ptr<char> p_ret = ((void *)0);
  if (p_getcwd_buf == 0)
  {
    vsf_secbuf_alloc(&p_getcwd_buf, VSFTP_PATH_MAX);
  }
  /* In case getcwd() fails */
  str_empty(p_str);
  p_ret = vsf_sysutil_getcwd(p_getcwd_buf, VSFTP_PATH_MAX);
  if (p_ret != 0)
  {
    str_alloc_text(p_str, p_getcwd_buf);
  }
}

int
str_write_loop(_Ptr<const struct mystr> p_str, const int fd)
{
  return vsf_sysutil_write_loop(fd, str_getbuf(p_str), str_getlen(p_str));
}

int
str_read_loop(_Ptr<struct mystr> p_str, const int fd)
{
  return vsf_sysutil_read_loop(
    fd, (char*) str_getbuf(p_str), str_getlen(p_str));
}

int
str_mkdir(_Ptr<const struct mystr> p_str, const unsigned int mode)
{
  return vsf_sysutil_mkdir(str_getbuf(p_str), mode);
}

int
str_rmdir(_Ptr<const struct mystr> p_str)
{
  return vsf_sysutil_rmdir(str_getbuf(p_str));
}

int
str_unlink(_Ptr<const struct mystr> p_str)
{
  return vsf_sysutil_unlink(str_getbuf(p_str));
}

int
str_chdir(_Ptr<const struct mystr> p_str)
{
  return vsf_sysutil_chdir(str_getbuf(p_str));
}

int
str_open(_Ptr<const struct mystr> p_str, const enum EVSFSysStrOpenMode mode)
{
  enum EVSFSysUtilOpenMode open_mode = (enum EVSFSysUtilOpenMode)kVSFSysStrOpenUnknown;
  switch (mode)
  {
    case kVSFSysStrOpenReadOnly:
      open_mode = kVSFSysUtilOpenReadOnly;
      break;
    case kVSFSysStrOpenUnknown:
      /* Fall through */
    default:
      bug("unknown mode value in str_open");
      break;
  }
  return vsf_sysutil_open_file(str_getbuf(p_str), open_mode);
}

int
str_stat(_Ptr<const struct mystr> p_str, struct vsf_sysutil_statbuf **p_ptr /*unsafe itype*/ : itype(_Ptr<_Ptr<struct vsf_sysutil_statbuf>>))
{
  return vsf_sysutil_stat(str_getbuf(p_str), p_ptr);
}

int
str_lstat(_Ptr<const struct mystr> p_str, struct vsf_sysutil_statbuf **p_ptr /*unsafe itype*/ : itype(_Ptr<_Ptr<struct vsf_sysutil_statbuf>>))
{
  return vsf_sysutil_lstat(str_getbuf(p_str), p_ptr);
}

int
str_create_exclusive(_Ptr<const struct mystr> p_str)
{
  return vsf_sysutil_create_file_exclusive(str_getbuf(p_str));
}

int
str_create(_Ptr<const struct mystr> p_str)
{
  return vsf_sysutil_create_or_open_file(
      str_getbuf(p_str), tunable_file_open_mode);
}

int
str_chmod(_Ptr<const struct mystr> p_str, unsigned int mode)
{
  return vsf_sysutil_chmod(str_getbuf(p_str), mode);
}

int
str_rename(_Ptr<const struct mystr> p_from_str, _Ptr<const struct mystr> p_to_str)
{
  return vsf_sysutil_rename(str_getbuf(p_from_str), str_getbuf(p_to_str));
}

struct vsf_sysutil_dir *str_opendir(_Ptr<const struct mystr> p_str) /*unsafe itype*/ : itype(_Ptr<struct vsf_sysutil_dir>)
{
  return vsf_sysutil_opendir(str_getbuf(p_str));
}

void
str_next_dirent(_Ptr<struct mystr> p_filename_str, struct vsf_sysutil_dir *p_dir /*unsafe itype*/ : itype(_Ptr<struct vsf_sysutil_dir>))
{
  _Nt_array_ptr<const char> p_filename = (_Nt_array_ptr<const char>) vsf_sysutil_next_dirent(p_dir);
  str_empty(p_filename_str);
  if (p_filename != 0)
  {
    str_alloc_text(p_filename_str, p_filename);
  }
}

int
str_readlink(_Ptr<struct mystr> p_str, _Ptr<const struct mystr> p_filename_str)
{
  static char* p_readlink_buf;
  int retval;
  if (p_readlink_buf == 0)
  {
    vsf_secbuf_alloc(&p_readlink_buf, VSFTP_PATH_MAX);
  }
  /* In case readlink() fails */
  str_empty(p_str);
  /* Note: readlink(2) does not NULL terminate, but our wrapper does */
  retval = vsf_sysutil_readlink(str_getbuf(p_filename_str), p_readlink_buf,
                                VSFTP_PATH_MAX);
  if (vsf_sysutil_retval_is_error(retval))
  {
    return retval;
  }
  str_alloc_text(p_str, p_readlink_buf);
  return 0;
}

struct vsf_sysutil_user *str_getpwnam(_Ptr<const struct mystr> p_user_str) /*unsafe itype*/ : itype(_Ptr<struct vsf_sysutil_user>)
{
  return vsf_sysutil_getpwnam(str_getbuf(p_user_str));
}

void
str_syslog(_Ptr<const struct mystr> p_str, int severe)
{
  vsf_sysutil_syslog(str_getbuf(p_str), severe);
}

