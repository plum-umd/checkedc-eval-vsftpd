#ifndef VSF_SYSSTR_H
#define VSF_SYSSTR_H

/* Forward declarations */
struct mystr;
struct vsf_sysutil_statbuf;
struct vsf_sysutil_dir;
struct vsf_sysutil_user;

void str_getcwd(_Ptr<struct mystr> p_str);
int str_readlink(_Ptr<struct mystr> p_str, _Ptr<const struct mystr> p_filename_str);
int str_write_loop(_Ptr<const struct mystr> p_str, const int fd);
int str_read_loop(_Ptr<struct mystr> p_str, const int fd);
int str_mkdir(_Ptr<const struct mystr> p_str, const unsigned int mode);
int str_rmdir(_Ptr<const struct mystr> p_str);
int str_unlink(_Ptr<const struct mystr> p_str);
int str_chdir(_Ptr<const struct mystr> p_str);
enum EVSFSysStrOpenMode
{
  kVSFSysStrOpenUnknown = 0,
  kVSFSysStrOpenReadOnly = 1
};
int str_open(_Ptr<const struct mystr> p_str, const enum EVSFSysStrOpenMode mode);
int str_create(_Ptr<const struct mystr> p_str);
int str_create_exclusive(_Ptr<const struct mystr> p_str);
int str_chmod(_Ptr<const struct mystr> p_str, unsigned int mode);
int str_stat(_Ptr<const struct mystr> p_str, struct vsf_sysutil_statbuf** p_ptr);
int str_lstat(_Ptr<const struct mystr> p_str, struct vsf_sysutil_statbuf** p_ptr);
int str_rename(_Ptr<const struct mystr> p_from_str, _Ptr<const struct mystr> p_to_str);
struct vsf_sysutil_dir* str_opendir(_Ptr<const struct mystr> p_str);
void str_next_dirent(_Ptr<struct mystr> p_filename_str,
                     struct vsf_sysutil_dir* p_dir);

struct vsf_sysutil_user* str_getpwnam(_Ptr<const struct mystr> p_user_str);

void str_syslog(_Ptr<const struct mystr> p_str, int severe);

#endif /* VSF_SYSSTR_H */

