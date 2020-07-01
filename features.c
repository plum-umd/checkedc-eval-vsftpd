/*
 * Part of Very Secure FTPd
 * Licence: GPL v2
 * Author: Chris Evans
 * features.c
 *
 * Routines to tell the client what features we support.
 */

#include "features.h"
#include "ftpcodes.h"
#include "ftpcmdio.h"
#include "tunables.h"

void handle_feat(struct vsf_session* p_sess : itype(_Ptr<struct vsf_session>))
{
  vsf_cmdio_write_hyphen(p_sess, FTP_FEAT, ((const char *)((const char *)((const char *)"Features:"))));
  if (tunable_ssl_enable)
  {
    if (tunable_sslv2 || tunable_sslv3)
    {
      vsf_cmdio_write_raw(p_sess, ((const char *)((const char *)((const char *)" AUTH SSL\r\n"))));
    }
    if (tunable_tlsv1)
    {
      vsf_cmdio_write_raw(p_sess, ((const char *)((const char *)((const char *)" AUTH TLS\r\n"))));
    }
  }
  if (tunable_port_enable)
  {
    vsf_cmdio_write_raw(p_sess, ((const char *)((const char *)((const char *)" EPRT\r\n"))));
  }
  if (tunable_pasv_enable)
  {
    vsf_cmdio_write_raw(p_sess, ((const char *)((const char *)((const char *)" EPSV\r\n"))));
  }
  vsf_cmdio_write_raw(p_sess, ((const char *)((const char *)((const char *)" MDTM\r\n"))));
  if (tunable_pasv_enable)
  {
    vsf_cmdio_write_raw(p_sess, ((const char *)((const char *)((const char *)" PASV\r\n"))));
  }
  if (tunable_ssl_enable)
  {
    vsf_cmdio_write_raw(p_sess, ((const char *)((const char *)((const char *)" PBSZ\r\n"))));
    vsf_cmdio_write_raw(p_sess, ((const char *)((const char *)((const char *)" PROT\r\n"))));
  }
  vsf_cmdio_write_raw(p_sess, ((const char *)((const char *)((const char *)" REST STREAM\r\n"))));
  vsf_cmdio_write_raw(p_sess, ((const char *)((const char *)((const char *)" SIZE\r\n"))));
  vsf_cmdio_write_raw(p_sess, ((const char *)((const char *)((const char *)" TVFS\r\n"))));
  vsf_cmdio_write_raw(p_sess, ((const char *)((const char *)((const char *)" UTF8\r\n"))));
  vsf_cmdio_write(p_sess, FTP_FEAT, ((const char *)((const char *)((const char *)"End"))));
}

