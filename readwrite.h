#ifndef VSF_READWRITE_H
#define VSF_READWRITE_H

struct vsf_session;
struct mystr;

enum EVSFRWTarget
{
  kVSFRWControl = 1,
  kVSFRWData
};

int ftp_write_str(_Ptr<const struct vsf_session> p_sess, _Ptr<const struct mystr> p_str, enum EVSFRWTarget target);
int ftp_read_data(_Ptr<struct vsf_session> p_sess, char* p_buf, unsigned int len);
int ftp_write_data(_Ptr<const struct vsf_session> p_sess, const char* p_buf, unsigned int len);
int ftp_getline(_Ptr<struct vsf_session> p_sess, _Ptr<struct mystr> p_str, char* p_buf);

#endif /* VSF_READWRITE_H */

