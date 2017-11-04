#ifndef VSF_PRELOGIN_H
#define VSF_PRELOGIN_H

#pragma BOUNDS_CHECKED ON

struct vsf_session;

/* init_connection()
 * PURPOSE
 * Called as an entry point to FTP protocol processing, when a client connects.
 * This function will emit the FTP greeting, then start talking FTP protocol
 * to the client.
 * PARAMETERS
 * p_sess         - the current session object
 */
void init_connection(_Ptr<struct vsf_session> p_sess);

#pragma BOUNDS_CHECKED OFF

#endif /* VSF_PRELOGIN_H */
