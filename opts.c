/*
 * Part of Very Secure FTPd
 * Licence: GPL v2
 * Author: Chris Evans
 * opts.c
 *
 * Routines to handle OPTS.
 */

#include "ftpcodes.h"
#include "ftpcmdio.h"
#include "session.h"
#include "opts.h"

void handle_opts(struct vsf_session* p_sess : itype(_Ptr<struct vsf_session>))
{
  str_upper(&p_sess->ftp_arg_str);
  if (str_equal_text(&p_sess->ftp_arg_str, ((const char *)((const char *)"UTF8 ON"))))
  {
    vsf_cmdio_write(p_sess, FTP_OPTSOK, ((const char *)((const char *)"Always in UTF8 mode.")));
  }
  else
  {
    vsf_cmdio_write(p_sess, FTP_BADOPTS, ((const char *)((const char *)"Option not understood.")));
  }
}

