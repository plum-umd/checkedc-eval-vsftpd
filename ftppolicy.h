#ifndef VSF_FTPPOLICY_H
#define VSF_FTPPOLICY_H

#pragma BOUNDS_CHECKED ON

/* Forward delcarations */
struct pt_sandbox;
struct vsf_session;

/* policy_setup()
 * PURPOSE
 * Sets up a sandbox policy according to the currently enabled options.
 * PARAMETERS
 * p_sandbox    - the sandbox to set the policy on
 * p_sess       - current vsftpd session object
 */
void policy_setup(_Ptr<struct pt_sandbox> p_sandbox,
                  _Ptr<const struct vsf_session> p_sess);

#pragma BOUNDS_CHECKED OFF

#endif /* VSF_FTPPOLICY_H */
