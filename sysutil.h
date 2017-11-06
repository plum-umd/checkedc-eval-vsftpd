#ifndef VSF_SYSUTIL_H
#define VSF_SYSUTIL_H

/* TODO: these functions need proper documenting! */

#ifndef VSF_FILESIZE_H
#include "filesize.h"
#endif

/* Return value queries */
int vsf_sysutil_retval_is_error(int retval);
enum EVSFSysUtilError
{
  kVSFSysUtilErrUnknown = 1,
  kVSFSysUtilErrADDRINUSE,
  kVSFSysUtilErrNOSYS,
  kVSFSysUtilErrINTR,
  kVSFSysUtilErrINVAL,
  kVSFSysUtilErrOPNOTSUPP,
  kVSFSysUtilErrACCES,
  kVSFSysUtilErrNOENT
};
enum EVSFSysUtilError vsf_sysutil_get_error(void);

/* Signal handling utility functions */
enum EVSFSysUtilSignal
{
  kVSFSysUtilSigALRM = 1,
  kVSFSysUtilSigTERM,
  kVSFSysUtilSigCHLD,
  kVSFSysUtilSigPIPE,
  kVSFSysUtilSigURG,
  kVSFSysUtilSigHUP
};
enum EVSFSysUtilInterruptContext
{
  kVSFSysUtilUnknown,
  kVSFSysUtilIO
};
/* typedef void (*vsf_sighandle_t)(void*); */
/* typedef void (*vsf_async_sighandle_t)(int); */
/* typedef void (*vsf_context_io_t)(int, int, void*); */
typedef _Ptr<void (_Ptr<void>)> vsf_sighandle_t;
typedef _Ptr<void (int)> vsf_async_sighandle_t;
typedef _Ptr<void (int, int, _Ptr<void>)> vsf_context_io_t;

void vsf_sysutil_install_null_sighandler(const enum EVSFSysUtilSignal sig);
void vsf_sysutil_install_sighandler(const enum EVSFSysUtilSignal,
                                    vsf_sighandle_t handler,
                                    _Ptr<void> p_private,
                                    int use_alarm);
void vsf_sysutil_install_async_sighandler(const enum EVSFSysUtilSignal sig,
                                          vsf_async_sighandle_t handler);
void vsf_sysutil_default_sig(const enum EVSFSysUtilSignal p_handlefuncsig);
void vsf_sysutil_install_io_handler(vsf_context_io_t handler, _Ptr<void> p_private);
void vsf_sysutil_uninstall_io_handler(void);
void vsf_sysutil_check_pending_actions(
  const enum EVSFSysUtilInterruptContext context, int retval, int fd);
void vsf_sysutil_block_sig(const enum EVSFSysUtilSignal sig);
void vsf_sysutil_unblock_sig(const enum EVSFSysUtilSignal sig);

/* Alarm setting/clearing utility functions */
void vsf_sysutil_set_alarm(const unsigned int trigger_seconds);
void vsf_sysutil_clear_alarm(void);

/* Directory related things */
_Nt_array_ptr<char> vsf_sysutil_getcwd(_Array_ptr<char> p_dest : count(buf_size),
				       const unsigned int buf_size) : count(buf_size);
int vsf_sysutil_mkdir(_Nt_array_ptr<const char> p_dirname, const unsigned int mode);
int vsf_sysutil_rmdir(_Nt_array_ptr<const char> p_dirname);
int vsf_sysutil_chdir(_Nt_array_ptr<const char> p_dirname);
int vsf_sysutil_rename(_Nt_array_ptr<const char> p_from, _Nt_array_ptr<const char> p_to);

struct vsf_sysutil_dir;
_Ptr<struct vsf_sysutil_dir> vsf_sysutil_opendir(_Nt_array_ptr<const char> p_dirname);
void vsf_sysutil_closedir(_Ptr<struct vsf_sysutil_dir> p_dir);
_Nt_array_ptr<const char> vsf_sysutil_next_dirent(_Ptr<struct vsf_sysutil_dir> p_dir);

/* File create/open/close etc. */
enum EVSFSysUtilOpenMode
{
  kVSFSysUtilOpenReadOnly = 1,
  kVSFSysUtilOpenWriteOnly,
  kVSFSysUtilOpenReadWrite
};
int vsf_sysutil_open_file(_Nt_array_ptr<const char> p_filename,
                          const enum EVSFSysUtilOpenMode);
/* Fails if file already exists */
int vsf_sysutil_create_file_exclusive(_Nt_array_ptr<const char> p_filename);
/* Creates file or appends if already exists */
int vsf_sysutil_create_or_open_file_append(_Nt_array_ptr<const char> p_filename,
                                           unsigned int mode);
/* Creates or appends */
int vsf_sysutil_create_or_open_file(_Nt_array_ptr<const char> p_filename, unsigned int mode);
void vsf_sysutil_dupfd2(int old_fd, int new_fd);
void vsf_sysutil_close(int fd);
int vsf_sysutil_close_failok(int fd);
int vsf_sysutil_unlink(_Nt_array_ptr<const char> p_dead);
int vsf_sysutil_write_access(_Nt_array_ptr<const char> p_filename);
void vsf_sysutil_ftruncate(int fd);

/* Reading and writing */
void vsf_sysutil_lseek_to(const int fd, filesize_t seek_pos);
void vsf_sysutil_lseek_end(const int fd);
filesize_t vsf_sysutil_get_file_offset(const int file_fd);
int vsf_sysutil_read(const int fd, _Array_ptr<char> p_buf : byte_count(size), const unsigned int size);
int vsf_sysutil_write(const int fd, _Array_ptr<const char> p_buf : byte_count(size),
                      const unsigned int size);
/* Reading and writing, with handling of interrupted system calls and partial
 * reads/writes. Slightly more usable than the standard UNIX API!
 */
int vsf_sysutil_read_loop(const int fd, _Array_ptr<void> p_buf : byte_count(size), unsigned int size);
int vsf_sysutil_write_loop(const int fd, _Array_ptr<const void> p_buf : byte_count(size), unsigned int size);

struct vsf_sysutil_statbuf;
int vsf_sysutil_stat(_Nt_array_ptr<const char> p_name, _Ptr<_Ptr<struct vsf_sysutil_statbuf>> p_ptr);
int vsf_sysutil_lstat(_Nt_array_ptr<const char> p_name, _Ptr<_Ptr<struct vsf_sysutil_statbuf>> p_ptr);
void vsf_sysutil_fstat(int fd, _Ptr<_Ptr<struct vsf_sysutil_statbuf>> p_ptr);
void vsf_sysutil_dir_stat(const _Ptr<struct vsf_sysutil_dir> p_dir,
                          _Ptr<_Ptr<struct vsf_sysutil_statbuf>> p_ptr);
int vsf_sysutil_statbuf_is_regfile(const _Ptr<struct vsf_sysutil_statbuf> p_stat);
int vsf_sysutil_statbuf_is_symlink(const _Ptr<struct vsf_sysutil_statbuf> p_stat);
int vsf_sysutil_statbuf_is_socket(const _Ptr<struct vsf_sysutil_statbuf> p_stat);
int vsf_sysutil_statbuf_is_dir(const _Ptr<struct vsf_sysutil_statbuf> p_stat);
filesize_t vsf_sysutil_statbuf_get_size(
  const _Ptr<struct vsf_sysutil_statbuf> p_stat);
_Nt_array_ptr<const char> vsf_sysutil_statbuf_get_perms(
  const _Ptr<struct vsf_sysutil_statbuf> p_stat);
_Nt_array_ptr<const char> vsf_sysutil_statbuf_get_date(
  const _Ptr<struct vsf_sysutil_statbuf> p_stat, int use_localtime, long curr_time);
_Nt_array_ptr<const char> vsf_sysutil_statbuf_get_numeric_date(
  const _Ptr<struct vsf_sysutil_statbuf> p_stat, int use_localtime);
unsigned int vsf_sysutil_statbuf_get_links(
  const _Ptr<struct vsf_sysutil_statbuf> p_stat);
int vsf_sysutil_statbuf_get_uid(const _Ptr<struct vsf_sysutil_statbuf> p_stat);
int vsf_sysutil_statbuf_get_gid(const _Ptr<struct vsf_sysutil_statbuf> p_stat);
int vsf_sysutil_statbuf_is_readable_other(
  const _Ptr<struct vsf_sysutil_statbuf> p_stat);
_Nt_array_ptr<const char> vsf_sysutil_statbuf_get_sortkey_mtime(
  const _Ptr<struct vsf_sysutil_statbuf> p_stat);

int vsf_sysutil_chmod(_Nt_array_ptr<const char> p_filename, unsigned int mode);
void vsf_sysutil_fchown(const int fd, const int uid, const int gid);
void vsf_sysutil_fchmod(const int fd, unsigned int mode);
int vsf_sysutil_readlink(_Nt_array_ptr<const char> p_filename, _Nt_array_ptr<char> p_dest,
                         unsigned int bufsiz);

/* Get / unget various locks. Lock gets are blocking. Write locks are
 * exclusive; read locks are shared.
 */
int vsf_sysutil_lock_file_write(int fd);
int vsf_sysutil_lock_file_read(int fd);
void vsf_sysutil_unlock_file(int fd);

/* Mapping/unmapping */
enum EVSFSysUtilMapPermission
{
  kVSFSysUtilMapProtReadOnly = 1,
  kVSFSysUtilMapProtNone
};
void vsf_sysutil_memprotect(void* p_addr, unsigned int len,
                            const enum EVSFSysUtilMapPermission perm);
void vsf_sysutil_memunmap(void* p_start, unsigned int length);

/* Memory allocating/freeing */
void* vsf_sysutil_malloc(unsigned int size) : byte_count(size);
void* vsf_sysutil_realloc(void* p_ptr : byte_count(1), unsigned int size) : byte_count(size);
void vsf_sysutil_free(const void* p_ptr : byte_count(1));

/* Process creation/exit/process handling */
unsigned int vsf_sysutil_getpid(void);
void vsf_sysutil_post_fork(void);
int vsf_sysutil_fork(void);
int vsf_sysutil_fork_failok(void);
void vsf_sysutil_exit(int exit_code);
struct vsf_sysutil_wait_retval
{
  int PRIVATE_HANDS_OFF_syscall_retval;
  int PRIVATE_HANDS_OFF_exit_status;
};
struct vsf_sysutil_wait_retval vsf_sysutil_wait(void);
int vsf_sysutil_wait_reap_one(void);
int vsf_sysutil_wait_get_retval(
  _Ptr<const struct vsf_sysutil_wait_retval> p_waitret);
int vsf_sysutil_wait_exited_normally(
  _Ptr<const struct vsf_sysutil_wait_retval> p_waitret);
int vsf_sysutil_wait_get_exitcode(
  _Ptr<const struct vsf_sysutil_wait_retval> p_waitret);

/* Various string functions */
#define vsf_sysutil_strlen_alt(p,s,len) \
  unsigned int len = 0; \
  _Nt_array_ptr<const char> s : count(len) = p; \
  for (; s[len]; len++);
  
unsigned int vsf_sysutil_strlen(_Nt_array_ptr<const char> p_text); 
_Nt_array_ptr<char> vsf_sysutil_strdup(_Nt_array_ptr<const char> p_str);
void vsf_sysutil_memclr(void* p_dest : byte_count(size), unsigned int size);
void vsf_sysutil_memcpy(void* p_dest : byte_count(size),
			const void* p_src : byte_count(size),
                        const unsigned int size);
void vsf_sysutil_strcpy(_Nt_array_ptr<char> p_dest,
			_Nt_array_ptr<const char> p_src,
			unsigned int maxsize);
int vsf_sysutil_memcmp(const void* p_src1 : byte_count(size),
		       const void* p_src2 : byte_count(size),
		       unsigned int size);
int vsf_sysutil_strcmp(_Nt_array_ptr<const char> p_src1,
		       _Nt_array_ptr<const char> p_src2);
int vsf_sysutil_atoi(_Nt_array_ptr<const char> p_str);
filesize_t vsf_sysutil_a_to_filesize_t(_Nt_array_ptr<const char> p_str);
_Nt_array_ptr<const char> vsf_sysutil_ulong_to_str(unsigned long the_ulong);
_Nt_array_ptr<const char> vsf_sysutil_filesize_t_to_str(filesize_t the_filesize);
_Nt_array_ptr<const char> vsf_sysutil_double_to_str(double the_double);
_Nt_array_ptr<const char> vsf_sysutil_uint_to_octal(unsigned int the_uint);
unsigned int vsf_sysutil_octal_to_uint(_Nt_array_ptr<const char> p_str);
int vsf_sysutil_toupper(int the_char);
int vsf_sysutil_isspace(int the_char);
int vsf_sysutil_isprint(int the_char);
int vsf_sysutil_isalnum(int the_char);
int vsf_sysutil_isdigit(int the_char);

/* Socket handling */
struct vsf_sysutil_sockaddr;
struct vsf_sysutil_socketpair_retval
{
  int socket_one;
  int socket_two;
};
void vsf_sysutil_sockaddr_alloc(_Ptr<_Ptr<struct vsf_sysutil_sockaddr>> p_sockptr);
void vsf_sysutil_sockaddr_clear(_Ptr<_Ptr<struct vsf_sysutil_sockaddr>> p_sockptr);
void vsf_sysutil_sockaddr_alloc_ipv4(_Ptr<_Ptr<struct vsf_sysutil_sockaddr>> p_sockptr);
void vsf_sysutil_sockaddr_alloc_ipv6(_Ptr<_Ptr<struct vsf_sysutil_sockaddr>> p_sockptr);
void vsf_sysutil_sockaddr_clone(_Ptr<_Ptr<struct vsf_sysutil_sockaddr>> p_sockptr,
  const _Ptr<struct vsf_sysutil_sockaddr> p_src);
int vsf_sysutil_sockaddr_addr_equal(const _Ptr<struct vsf_sysutil_sockaddr> p1,
                                    const _Ptr<struct vsf_sysutil_sockaddr> p2);
int vsf_sysutil_sockaddr_is_ipv6(
  const _Ptr<struct vsf_sysutil_sockaddr> p_sockaddr);
void vsf_sysutil_sockaddr_set_ipv4addr(_Ptr<struct vsf_sysutil_sockaddr> p_sockptr,
                                       _Array_ptr<const unsigned char> p_raw : count(4));
void vsf_sysutil_sockaddr_set_ipv6addr(_Ptr<struct vsf_sysutil_sockaddr> p_sockptr,
				       _Array_ptr<const unsigned char> p_raw : count(16));
void vsf_sysutil_sockaddr_set_any(_Ptr<struct vsf_sysutil_sockaddr> p_sockaddr);
unsigned short vsf_sysutil_sockaddr_get_port(
    const _Ptr<struct vsf_sysutil_sockaddr> p_sockptr);
void vsf_sysutil_sockaddr_set_port(_Ptr<struct vsf_sysutil_sockaddr> p_sockptr,
                                   unsigned short the_port);
int vsf_sysutil_is_port_reserved(unsigned short port);
int vsf_sysutil_get_ipsock(const _Ptr<struct vsf_sysutil_sockaddr> p_sockaddr);
unsigned int vsf_sysutil_get_ipaddr_size(void);
_Ptr<void> vsf_sysutil_sockaddr_get_raw_addr(
  _Ptr<struct vsf_sysutil_sockaddr> p_sockaddr);
_Ptr<const void> vsf_sysutil_sockaddr_ipv6_v4(
  const _Ptr<struct vsf_sysutil_sockaddr> p_sockaddr);
_Ptr<const void> vsf_sysutil_sockaddr_ipv4_v6(
  const _Ptr<struct vsf_sysutil_sockaddr> p_sockaddr);
int vsf_sysutil_get_ipv4_sock(void);
int vsf_sysutil_get_ipv6_sock(void);
struct vsf_sysutil_socketpair_retval
  vsf_sysutil_unix_stream_socketpair(void);
int vsf_sysutil_bind(int fd, const _Ptr<struct vsf_sysutil_sockaddr> p_sockptr);
int vsf_sysutil_listen(int fd, const unsigned int backlog);
void vsf_sysutil_getsockname(int fd, _Ptr<_Ptr<struct vsf_sysutil_sockaddr>> p_sockptr);
void vsf_sysutil_getpeername(int fd, _Ptr<_Ptr<struct vsf_sysutil_sockaddr>> p_sockptr);
int vsf_sysutil_accept_timeout(int fd, _Ptr<struct vsf_sysutil_sockaddr> p_sockaddr,
                               unsigned int wait_seconds);
int vsf_sysutil_connect_timeout(int fd,
                                const _Ptr<struct vsf_sysutil_sockaddr> p_sockaddr,
                                unsigned int wait_seconds);
void vsf_sysutil_dns_resolve(_Ptr<_Ptr<struct vsf_sysutil_sockaddr>> p_sockptr,
                             _Nt_array_ptr<const char> p_name: count(0));
/* Option setting on sockets */
void vsf_sysutil_activate_keepalive(int fd);
void vsf_sysutil_set_iptos_throughput(int fd);
void vsf_sysutil_activate_reuseaddr(int fd);
void vsf_sysutil_set_nodelay(int fd);
void vsf_sysutil_activate_sigurg(int fd);
void vsf_sysutil_activate_oobinline(int fd);
void vsf_sysutil_activate_linger(int fd);
void vsf_sysutil_deactivate_linger_failok(int fd);
void vsf_sysutil_activate_noblock(int fd);
void vsf_sysutil_deactivate_noblock(int fd);
/* This does SHUT_RDWR */
void vsf_sysutil_shutdown_failok(int fd);
/* And this does SHUT_RD */
void vsf_sysutil_shutdown_read_failok(int fd);
int vsf_sysutil_recv_peek(const int fd, void* p_buf : byte_count(len), unsigned int len);

_Nt_array_ptr<const char> vsf_sysutil_inet_ntop(
  const _Ptr<struct vsf_sysutil_sockaddr> p_sockptr);
_Nt_array_ptr<const char>
vsf_sysutil_inet_ntoa(const void* p_raw_addr : itype(_Ptr<const void>));
int vsf_sysutil_inet_aton(_Nt_array_ptr<const char> p_text,
			  _Ptr<struct vsf_sysutil_sockaddr> p_addr);

/* User database queries etc. */
struct vsf_sysutil_user;
struct vsf_sysutil_group;

_Ptr<struct vsf_sysutil_user> vsf_sysutil_getpwuid(const int uid);
_Ptr<struct vsf_sysutil_user> vsf_sysutil_getpwnam(_Nt_array_ptr<const char> p_user);
_Nt_array_ptr<const char> vsf_sysutil_user_getname(const _Ptr<struct vsf_sysutil_user> p_user);
_Nt_array_ptr<const char> vsf_sysutil_user_get_homedir(
  const _Ptr<struct vsf_sysutil_user> p_user);
int vsf_sysutil_user_getuid(const _Ptr<struct vsf_sysutil_user> p_user);
int vsf_sysutil_user_getgid(const _Ptr<struct vsf_sysutil_user> p_user);

_Ptr<struct vsf_sysutil_group> vsf_sysutil_getgrgid(const int gid);
_Nt_array_ptr<const char> vsf_sysutil_group_getname(const _Ptr<struct vsf_sysutil_group> p_group);

/* More random things */
unsigned int vsf_sysutil_getpagesize(void);
unsigned char vsf_sysutil_get_random_byte(void);
unsigned int vsf_sysutil_get_umask(void);
void vsf_sysutil_set_umask(unsigned int umask);
void vsf_sysutil_make_session_leader(void);
void vsf_sysutil_reopen_standard_fds(void);
void vsf_sysutil_tzset(void);
_Nt_array_ptr<const char> vsf_sysutil_get_current_date(void);
void vsf_sysutil_qsort(_Array_ptr<void> p_base /* : count(num_elem) */,
		       unsigned int num_elem,
                       unsigned int elem_size,
                       _Ptr<int (_Ptr<const void>, _Ptr<const void>)> p_compar);
_Nt_array_ptr<char> vsf_sysutil_getenv(_Nt_array_ptr<const char> p_var);
typedef void (*exitfunc_t)(void);
void vsf_sysutil_set_exit_func(exitfunc_t exitfunc);
int vsf_sysutil_getuid(void);

/* Syslogging (bah) */
void vsf_sysutil_openlog(int force);
void vsf_sysutil_syslog(_Nt_array_ptr<const char> p_text, int severe);
void vsf_sysutil_closelog(void);

/* Credentials handling */
int vsf_sysutil_running_as_root(void);
void vsf_sysutil_setuid(const _Ptr<struct vsf_sysutil_user> p_user);
void vsf_sysutil_setgid(const _Ptr<struct vsf_sysutil_user> p_user);
void vsf_sysutil_setuid_numeric(int uid);
void vsf_sysutil_setgid_numeric(int gid);
int vsf_sysutil_geteuid(void);
int vsf_sysutil_getegid(void);
void vsf_sysutil_seteuid(const _Ptr<struct vsf_sysutil_user> p_user);
void vsf_sysutil_setegid(const _Ptr<struct vsf_sysutil_user> p_user);
void vsf_sysutil_seteuid_numeric(int uid);
void vsf_sysutil_setegid_numeric(int gid);
void vsf_sysutil_clear_supp_groups(void);
void vsf_sysutil_initgroups(const _Ptr<struct vsf_sysutil_user> p_user);
void vsf_sysutil_chroot(_Nt_array_ptr<const char> p_root_path);

/* Time handling */
/* Do not call get_time_usec() without calling get_time_sec()
 * first otherwise you will get stale data.
 */
long vsf_sysutil_get_time_sec(void);
long vsf_sysutil_get_time_usec(void);
long vsf_sysutil_parse_time(_Nt_array_ptr<const char> p_text);
void vsf_sysutil_sleep(double seconds);
int vsf_sysutil_setmodtime(_Nt_array_ptr<const char> p_file,
			   long the_time, int is_localtime);

/* Limits */
void vsf_sysutil_set_address_space_limit(unsigned long bytes);
void vsf_sysutil_set_no_fds(void);
void vsf_sysutil_set_no_procs(void);

#endif /* VSF_SYSUTIL_H */

