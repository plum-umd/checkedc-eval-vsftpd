#ifndef VSF_IPADDRPARSE_H
#define VSF_IPADDRPARSE_H

#pragma BOUNDS_CHECKED ON

struct mystr;

/* Effectively doing the same sort of job as inet_pton. Since inet_pton does
 * a non-trivial amount of parsing, we'll do it ourselves for maximum security
 * and safety.
 */

_Nt_array_ptr<const unsigned char> vsf_sysutil_parse_ipv6(_Ptr<const struct mystr> p_str);

_Array_ptr<const unsigned char> vsf_sysutil_parse_ipv4(_Ptr<const struct mystr> p_str) : count(4);

_Array_ptr<const unsigned char>
vsf_sysutil_parse_uchar_string_sep(_Ptr<const struct mystr> p_str,
  char sep, _Array_ptr<unsigned char> p_items : count(items),
  unsigned int items) : count(items);

#pragma BOUNDS_CHECKED OFF

#endif /* VSF_IPADDRPARSE_H */

