#ifndef VSF_ONEPROCESS_H
#define VSF_ONEPROCESS_H

struct mystr;
struct vsf_session;

/* vsf_one_process_start()
 * PURPOSE
 * Called to start FTP login processing using the one process model. Before
 * processing starts, all possible privileges are dropped.
 * PARAMETERS
 * p_sess       - the current session object
 */
void vsf_one_process_start(_Ptr<struct vsf_session> p_sess);

/* vsf_one_process_login()
 * PURPOSE
 * Called to propose a login using the one process model. Only anonymous
 * logins supported!
 * PARAMETERS
 * p_sess       - the current session object
 * p_pass_str   - the proposed password
 */
void vsf_one_process_login(_Ptr<struct vsf_session> p_sess,
                           const _Ptr<struct mystr> p_pass_str);

/* vsf_one_process_get_priv_data_sock()
 * PURPOSE
 * Get a privileged port 20 bound data socket using the one process model.
 * PARAMETERS
 * p_sess       - the current session object
 * RETURNS
 * The file descriptor of the privileged socket
 */
int vsf_one_process_get_priv_data_sock(_Ptr<struct vsf_session> p_sess);

/* vsf_one_process_pasv_cleanup()
 * PURPOSE
 * Clean up any listening passive socket.
 * PARAMETERS
 * p_sess       - the current session object
 */
void vsf_one_process_pasv_cleanup(_Ptr<struct vsf_session> p_sess);

/* vsf_one_process_pasv_active()
 * PURPOSE
 * Determine whether a listening pasv socket is active.
 * PARAMETERS
 * p_sess       - the current session object
 * RETURNS
 * 1 if active, 0 if not.
 */
int vsf_one_process_pasv_active(_Ptr<struct vsf_session> p_sess);

/* vsf_one_process_listen()
 * PURPOSE
 * Start listening for an incoming connection.
 * PARAMETERS
 * p_sess       - the current session object
 * RETURNS
 * The port we listened on.
 */
unsigned short vsf_one_process_listen(_Ptr<struct vsf_session> p_sess);

/* vsf_one_process_get_pasv_fd()
 * PURPOSE
 * Accept an incoming connection.
 * PARAMETERS
 * p_sess       - the current session object
 * RETURNS
 * The file descriptor for the incoming connection.
 */
int vsf_one_process_get_pasv_fd(_Ptr<struct vsf_session> p_sess);

/* vsf_one_process_chown_upload()
 * PURPOSE
 * Change ownership of an uploaded file using the one process model.
 * PARAMETERS
 * p_sess       - the current session object
 * fd           - the file descriptor to change ownership on
 */
void vsf_one_process_chown_upload(_Ptr<struct vsf_session> p_sess, int fd);

#endif /* VSF_ONEPROCESS_H */

