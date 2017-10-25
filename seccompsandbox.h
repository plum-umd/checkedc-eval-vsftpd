#ifndef VSF_SECCOMPSANDBOX_H
#define VSF_SECCOMPSANDBOX_H

struct vsf_session;

void seccomp_sandbox_init();

void seccomp_sandbox_setup_prelogin(_Ptr<const struct vsf_session> p_sess);

void seccomp_sandbox_setup_postlogin(_Ptr<const struct vsf_session> p_sess);

void seccomp_sandbox_setup_postlogin_broker();

void seccomp_sandbox_lockdown();

#endif /* VSF_SECCOMPSANDBOX_H */

