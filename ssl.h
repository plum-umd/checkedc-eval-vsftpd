#ifndef VSF_SSL_H
#define VSF_SSL_H

struct vsf_session;
struct mystr;

int ssl_read(_Ptr<struct vsf_session> p_sess, void* p_ssl, char *p_buf : itype(_Ptr<char>), unsigned int len);
int ssl_peek(_Ptr<struct vsf_session> p_sess, void* p_ssl, char *p_buf : itype(_Ptr<char>), unsigned int len);
int ssl_write(void* p_ssl, const char *p_buf : itype(_Ptr<const char>), unsigned int len);
int ssl_write_str(void* p_ssl, _Ptr<const struct mystr> p_str);
int ssl_read_into_str(_Ptr<struct vsf_session> p_sess, void* p_ssl, _Ptr<struct mystr> p_str);
void ssl_init(_Ptr<struct vsf_session> p_sess);
int ssl_accept(_Ptr<struct vsf_session> p_sess, int fd);
int ssl_data_close(struct vsf_session *p_sess : itype(_Ptr<struct vsf_session>));
void ssl_comm_channel_init(struct vsf_session *p_sess : itype(_Ptr<struct vsf_session>));
void ssl_comm_channel_set_consumer_context(struct vsf_session *p_sess : itype(_Ptr<struct vsf_session>));
void ssl_comm_channel_set_producer_context(struct vsf_session *p_sess : itype(_Ptr<struct vsf_session>));
void handle_auth(struct vsf_session *p_sess : itype(_Ptr<struct vsf_session>));
void handle_pbsz(struct vsf_session *p_sess : itype(_Ptr<struct vsf_session>));
void handle_prot(struct vsf_session *p_sess : itype(_Ptr<struct vsf_session>));
void ssl_control_handshake(struct vsf_session *p_sess : itype(_Ptr<struct vsf_session>));
void ssl_add_entropy(_Ptr<struct vsf_session> p_sess);

#endif /* VSF_SSL_H */

