#ifndef VSF_BANNER_H
#define VSF_BANNER_H

#pragma BOUNDS_CHECKED ON

struct vsf_session;
struct mystr;

/* vsf_banner_dir_changed()
 * PURPOSE
 * This function, when called, will check if the current directory has just
 * been entered for the first time in this session. If so, and message file
 * support is on, a message file is looked for (default .message), and output
 * to the FTP control connection with the FTP code prefix specified by
 * "ftpcode".
 * PARAMETERS
 * p_sess         - the current FTP session object
 * ftpcode        - the FTP code to show with the message
 */
void vsf_banner_dir_changed(_Ptr<struct vsf_session> p_sess, int ftpcode);

/* vsf_banner_write()
 * PURPOSE
 * This function, when called, will write the specified string as a multiline
 * FTP banner, using the specified FTP response code.
 * PARAMETERS
 * p_sess         - the current FTP session object
 * p_str          - the string to write
 * ftpcode        - the FTP code to show with the message
 */
void vsf_banner_write(_Ptr<struct vsf_session> p_sess, _Ptr<struct mystr> p_str,
                      int ftpcode);

#pragma BOUNDS_CHECKED OFF

#endif /* VSF_BANNER_H */

