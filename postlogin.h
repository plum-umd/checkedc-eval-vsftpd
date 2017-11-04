#ifndef VSF_POSTLOGIN_H
#define VSF_POSTLOGIN_H

#pragma BOUNDS_CHECKED ON

struct vsf_session;

/* process_post_login()
 * PURPOSE
 * Called to begin FTP protocol parsing for a logged in session.
 * PARAMETERS
 * p_sess       - the current session object
 */
void process_post_login(_Ptr<struct vsf_session> p_sess);

#pragma BOUNDS_CHECKED OFF

#endif /* VSF_POSTLOGIN_H */

