/*
 * Part of Very Secure FTPd
 * Licence: GPL v2
 * Author: Chris Evans
 * utility.c
 */

#include "utility.h"
#include "sysutil.h"
#include "str.h"
#include "defs.h"

#define DIE_DEBUG

void
die(_Nt_array_ptr<const char> p_text : count(0))
{
#ifdef DIE_DEBUG
  bug(p_text);
#endif
  vsf_sysutil_exit(2);
}

void
die2(_Nt_array_ptr<const char> p_text1 : count(0),
     _Nt_array_ptr<const char> p_text2 : count(0))
{
  struct mystr die_str = INIT_MYSTR;
  str_alloc_text(&die_str, _Assume_bounds_cast<_Nt_array_ptr<const char>>(p_text1,0));
  if (p_text2)
  {
    str_append_text(&die_str, _Assume_bounds_cast<_Nt_array_ptr<const char>>(p_text2,0));
  }
  else
  {
    str_append_text(&die_str, "(null)");
  }
  die(str_getbuf(&die_str));
}

void
bug(_Nt_array_ptr<const char> p_text : count(0))
{
  /* Rats. Try and write the reason to the network for diagnostics */
  vsf_sysutil_activate_noblock(VSFTP_COMMAND_FD);
  (void) vsf_sysutil_write_loop(VSFTP_COMMAND_FD, "500 OOPS: ", 10);
  (void) vsf_sysutil_write_loop(VSFTP_COMMAND_FD, (const char *)p_text,
                                vsf_sysutil_strlen(p_text));
  (void) vsf_sysutil_write_loop(VSFTP_COMMAND_FD, "\r\n", 2);
  vsf_sysutil_exit(2);
}

void
vsf_exit(_Nt_array_ptr<const char> p_text : count(0))
{
  (void) vsf_sysutil_write_loop(VSFTP_COMMAND_FD, (const char *)p_text,
                                vsf_sysutil_strlen(p_text));
  vsf_sysutil_exit(0);
}

