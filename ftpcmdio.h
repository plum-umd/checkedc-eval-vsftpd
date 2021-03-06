#ifndef VSF_FTPCMDIO_H
#define VSF_FTPCMDIO_H

#pragma BOUNDS_CHECKED ON

struct mystr;
struct vsf_session;

/* vsf_cmdio_sock_setup()
 * PURPOSE
 * Initialise a few socket settings (keepalive, nonagle, etc). on the FTP
 * control connection.
 */
void vsf_cmdio_sock_setup(void);

/* vsf_cmdio_write()
 * PURPOSE
 * Write a response to the FTP control connection.
 * PARAMETERS
 * p_sess       - the current session object
 * status       - the status code to report
 * p_text       - the text to report
 */
void vsf_cmdio_write(_Ptr<struct vsf_session> p_sess, int status,
		     _Nt_array_ptr<const char> p_text);

/* vsf_cmdio_write_hyphen()
 * PURPOSE
 * Write a response to the FTP control connection, with a hyphen '-'
 * continuation indicator.
 * PARAMETERS
 * p_sess       - the current session object
 * status       - the status code to report
 * p_text       - the text to report
 */
void vsf_cmdio_write_hyphen(_Ptr<struct vsf_session> p_sess, int status,
			    _Nt_array_ptr<const char> p_text);

/* vsf_cmdio_write_raw()
 * PURPOSE
 * Write a raw response to the FTP control connection. A status code is
 * not prepended, and it is also the client's responsibility to include
 * newline characters if required.
 * PARAMETERS
 * p_sess       - the current session object
 * p_text       - the text to report
 */
void vsf_cmdio_write_raw(_Ptr<struct vsf_session> p_sess,
			 _Nt_array_ptr<const char> p_text);

/* vsf_cmdio_write_exit()
 * PURPOSE
 * The same as vsf_cmdio_write(), and then the calling process is exited. The
 * write is _guaranteed_ to not block (ditching output if neccessary).
 */
void vsf_cmdio_write_exit(_Ptr<struct vsf_session> p_sess, int status,
			  _Nt_array_ptr<const char> p_text,
			  int exit_val);

/* vsf_cmdio_write_str()
 * PURPOSE
 * The same as vsf_cmdio_write(), apart from the text is specified as a
 * string buffer object "p_str".
 */
void vsf_cmdio_write_str(_Ptr<struct vsf_session> p_sess, int status,
                         _Ptr<const struct mystr> p_str);

/* vsf_cmdio_write_str_hyphen()
 * PURPOSE
 * The same as vsf_cmdio_write_str(), apart from the response line is
 * output with the continuation indicator '-' between the response code and
 * the response text. This indicates there are more lines of response.
 */
void vsf_cmdio_write_str_hyphen(_Ptr<struct vsf_session> p_sess, int status,
                                _Ptr<const struct mystr> p_str);

/* vsf_cmdio_set_alarm()
 * PURPOSE
 * Activate the control connection inactivity timeout. This is explicitly
 * exposed in the API so that we can play it safe, and activate the alarm
 * before _any_ potentially blocking calls.
 * PARAMETERS
 * p_sess       - The current session object
 */
void vsf_cmdio_set_alarm(_Ptr<struct vsf_session> p_sess);

/* vsf_cmdio_get_cmd_and_arg()
 * PURPOSE
 * Read an FTP command (and optional argument) from the FTP control connection.
 * PARAMETERS
 * p_sess       - The current session object
 * p_cmd_str    - Where to put the FTP command string (may be empty)
 * p_arg_str    - Where to put the FTP argument string (may be empty)
 * set_alarm    - If true, the control connection inactivity monitor is used
 */
void vsf_cmdio_get_cmd_and_arg(_Ptr<struct vsf_session> p_sess,
                               _Ptr<struct mystr> p_cmd_str,
                               _Ptr<struct mystr> p_arg_str, int set_alarm);

#pragma BOUNDS_CHECKED OFF

#endif /* VSF_FTPCMDIO_H */

