#ifndef VSF_SSL_H
#define VSF_SSL_H

#pragma BOUNDS_CHECKED ON

struct vsf_session;
struct mystr;

int ssl_read(_Ptr<struct vsf_session> p_sess,
             _Ptr<void> p_ssl,
             _Array_ptr<char> p_buf : count(len),
             unsigned int len);
int ssl_peek(_Ptr<struct vsf_session> p_sess,
             _Ptr<void> p_ssl,
             _Array_ptr<char> p_buf : count(len),
             unsigned int len);
int ssl_write(_Ptr<void> p_ssl, _Array_ptr<const char> p_buf : count(len), unsigned int len);
int ssl_write_str(_Ptr<void> p_ssl, _Ptr<const struct mystr> p_str);
int ssl_read_into_str(_Ptr<struct vsf_session> p_sess,
                      _Ptr<void> p_ssl,
                      _Ptr<struct mystr> p_str);
void ssl_init(_Ptr<struct vsf_session> p_sess);
int ssl_accept(_Ptr<struct vsf_session> p_sess, int fd);
int ssl_data_close(_Ptr<struct vsf_session> p_sess);
void ssl_comm_channel_init(_Ptr<struct vsf_session> p_sess);
void ssl_comm_channel_set_consumer_context(_Ptr<struct vsf_session> p_sess);
void ssl_comm_channel_set_producer_context(_Ptr<struct vsf_session> p_sess);
void handle_auth(_Ptr<struct vsf_session> p_sess);
void handle_pbsz(_Ptr<struct vsf_session> p_sess);
void handle_prot(_Ptr<struct vsf_session> p_sess);
void ssl_control_handshake(_Ptr<struct vsf_session> p_sess);
void ssl_add_entropy(_Ptr<struct vsf_session> p_sess);

#pragma BOUNDS_CHECKED OFF

#endif /* VSF_SSL_H */

