#ifndef VSF_READWRITE_H
#define VSF_READWRITE_H

#include "defs.h"

struct vsf_session;
struct mystr;

enum EVSFRWTarget
{
  kVSFRWControl = 1,
  kVSFRWData
};

int ftp_write_str(_Ptr<const struct vsf_session> p_sess, _Ptr<const struct mystr> p_str,
                  enum EVSFRWTarget target);
int ftp_read_data(_Ptr<struct vsf_session> p_sess,
		  _Array_ptr<char> p_buf : count(len), unsigned int len);
int ftp_write_data(_Ptr<const struct vsf_session> p_sess,
		   _Array_ptr<const char> p_buf : count(len), unsigned int len);
int ftp_getline(_Ptr<struct vsf_session> p_sess, _Ptr<struct mystr> p_str, _Array_ptr<char> p_buf : count(VSFTP_MAX_COMMAND_LINE));


#endif /* VSF_READWRITE_H */

