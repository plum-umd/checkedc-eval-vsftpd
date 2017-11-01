/*
 * Part of Very Secure FTPd
 * Licence: GPL v2
 * Author: Chris Evans
 * str.c
 *
 * Generic string handling functions. The fact that a string is implemented
 * internally using a buffer is not exposed in the API. If you can't see
 * the buffers, you can't handle them in a screwed way. Or so goes the
 * theory, anyway...
 */

/* Anti-lamer measures deployed, sir! */
#define PRIVATE_HANDS_OFF_p_buf p_buf
#define PRIVATE_HANDS_OFF_len len
#define PRIVATE_HANDS_OFF_alloc_bytes alloc_bytes
#include "str.h"

/* Ick. Its for die() */
#include "utility.h"
#include "sysutil.h"

/* File local functions */
static void str_split_text_common(_Ptr<struct mystr> p_src, _Ptr<struct mystr> p_rhs,
                                  const char* p_text, int is_reverse);
static int
str_equal_internal(_Array_ptr<const char> p_buf1 : count(buf1_len),
		   unsigned int buf1_len,
                   _Array_ptr<const char> p_buf2 : count(buf2_len),
		   unsigned int buf2_len);

/* Private functions */
/* static void */
/* s_setbuf(_Ptr<struct mystr> p_str, char* p_newbuf) */
/* { */
/*   if (p_str->p_buf != 0) */
/*   { */
/*     bug("p_buf not NULL when setting it"); */
/*   } */
/*   p_str->p_buf = p_newbuf; */
/* } */

void
private_str_alloc_memchunk(_Ptr<struct mystr> p_str,
			   _Array_ptr<const char> p_src : count(len),
                           unsigned int len)
{
  /* Make sure this will fit in the buffer */
  unsigned int buf_needed;
  if (len + 1 < len)
  {
    bug("integer overflow");
  }
  buf_needed = len + 1;
  if (buf_needed > p_str->alloc_bytes)
  {
    str_free(p_str);
    /* s_setbuf(p_str, vsf_sysutil_malloc(buf_needed)); */
    p_str->p_buf = vsf_sysutil_malloc(buf_needed);
    p_str->alloc_bytes = buf_needed;
  }
  vsf_sysutil_memcpy(p_str->p_buf, p_src, len);
  p_str->p_buf[len] = '\0';
  p_str->len = len;
}

void
private_str_append_memchunk(_Ptr<struct mystr> p_str,
			    _Array_ptr<const char> p_src : count(len),
                            unsigned int len)
{
  unsigned int buf_needed;
  if (len + p_str->len < len)
  {
    bug("integer overflow");
  }
  buf_needed = len + p_str->len;
  if (buf_needed + 1 < buf_needed)
  {
    bug("integer overflow");
  }
  buf_needed++;
  if (buf_needed > p_str->alloc_bytes)
  {
    p_str->p_buf = vsf_sysutil_realloc(p_str->p_buf, buf_needed);
    p_str->alloc_bytes = buf_needed;
  }
  vsf_sysutil_memcpy(p_str->p_buf + p_str->len, p_src, len);
  p_str->p_buf[p_str->len + len] = '\0';
  p_str->len += len;
}

/* Public functions */
void
str_alloc_text(_Ptr<struct mystr> p_str, _Nt_array_ptr<const char> p_src : count(0))
{
  vsf_sysutil_strlen_alt(p_src,s,len);
  private_str_alloc_memchunk(p_str, s, len);
}

void
str_copy(_Ptr<struct mystr> p_dest, _Ptr<const struct mystr> p_src)
{
  private_str_alloc_memchunk(p_dest, p_src->p_buf, p_src->len);
}

_Nt_array_ptr<const char>
str_strdup(_Ptr<const struct mystr> p_str) : count(0)
{
  return vsf_sysutil_strdup(_Assume_bounds_cast<_Nt_array_ptr<const char>>(str_getbuf(p_str),0));
}

void
str_alloc_alt_term(_Ptr<struct mystr> p_str,
		   _Array_ptr<const char> p_src : count(maxlen),
		   char term, unsigned int maxlen)
{
  _Array_ptr<const char> p_search : count(maxlen) = p_src;
  _Array_ptr<const char> p_end : count(0) = p_src + maxlen;
  unsigned int len = 0;
  while (p_search < p_end && *p_search != term)
  {
    p_search++;
    len++;
    if (len == 0)
    {
      bug("integer overflow");
    }
  }
  private_str_alloc_memchunk(p_str, p_src, len);
}

void
str_alloc_ulong(_Ptr<struct mystr> p_str, unsigned long the_long)
{
  str_alloc_text(p_str, vsf_sysutil_ulong_to_str(the_long));
}

void
str_alloc_filesize_t(_Ptr<struct mystr> p_str, filesize_t the_filesize)
{
  str_alloc_text(p_str, vsf_sysutil_filesize_t_to_str(the_filesize));
}

void
str_free(_Ptr<struct mystr> p_str)
{
  if (p_str->p_buf != 0)
  {
    vsf_sysutil_free(p_str->p_buf);
  }
  p_str->p_buf = 0;
  p_str->len = 0;
  p_str->alloc_bytes = 0;
}

void
str_empty(_Ptr<struct mystr> p_str)
{
  /* Ensure a buffer is allocated. */
  (void) str_getbuf(p_str);
  str_trunc(p_str, 0);
}

void
str_trunc(_Ptr<struct mystr> p_str, unsigned int trunc_len)
{
  if (trunc_len >= p_str->alloc_bytes)
  {
    bug("trunc_len not smaller than alloc_bytes in str_trunc");
  }
  p_str->len = trunc_len;
  p_str->p_buf[p_str->len] = '\0';
}

void
str_reserve(_Ptr<struct mystr> p_str, unsigned int res_len)
{
  /* Reserve space for the trailing zero as well. */
  res_len++;
  if (res_len == 0)
  {
    bug("integer overflow");
  }
  if (res_len > p_str->alloc_bytes)
  {
    p_str->p_buf = vsf_sysutil_realloc(p_str->p_buf, res_len);
    p_str->alloc_bytes = res_len;
  }
  p_str->p_buf[res_len - 1] = '\0';
}

int
str_isempty(_Ptr<const struct mystr> p_str)
{
  return (p_str->len == 0);
}

unsigned int
str_getlen(_Ptr<const struct mystr> p_str)
{
  return p_str->len;
}

/* XXX - could make the count equal to the length? */
_Nt_array_ptr<const char>
str_getbuf(_Ptr<const struct mystr> p_str) : count(0)
{
  if (p_str->p_buf == 0)
  {
    if (p_str->len != 0 || p_str->alloc_bytes != 0)
    {
      bug("p_buf NULL and len or alloc_bytes != 0 in str_getbuf");
    }
    private_str_alloc_memchunk((struct mystr*)p_str, 0, 0);
  }
  return _Assume_bounds_cast<_Nt_array_ptr<const char>>(p_str->p_buf,0); 
}

int
str_strcmp(_Ptr<const struct mystr> p_str1, _Ptr<const struct mystr> p_str2)
{
  return str_equal_internal(p_str1->p_buf, p_str1->len,
                            p_str2->p_buf, p_str2->len);
}

static int
str_equal_internal(_Array_ptr<const char> p_buf1 : count(buf1_len),
		   unsigned int buf1_len,
                   _Array_ptr<const char> p_buf2 : count(buf2_len),
		   unsigned int buf2_len)
{
  int retval;
  unsigned int minlen = buf1_len;
  if (buf2_len < minlen)
  {
    minlen = buf2_len;
  }
  retval = vsf_sysutil_memcmp(p_buf1, p_buf2, minlen);
  if (retval != 0 || buf1_len == buf2_len)
  {
    return retval;
  }
  /* Strings equal but lengths differ. The greater one, then, is the longer */
  return (int) (buf1_len - buf2_len);
}

int
str_equal(_Ptr<const struct mystr> p_str1, _Ptr<const struct mystr> p_str2)
{
  return (str_strcmp(p_str1, p_str2) == 0);
}

int
str_equal_text(_Ptr<const struct mystr> p_str, const char* p_text)
{
  unsigned int cmplen = vsf_sysutil_strlen(p_text);
  _Array_ptr<const char> p_text_tmp : count(cmplen) =
    _Assume_bounds_cast<_Array_ptr<const char>>(p_text, cmplen);
  return (str_equal_internal(p_str->p_buf, p_str->len, p_text_tmp, cmplen) == 0);
}

void
str_append_str(_Ptr<struct mystr> p_str, _Ptr<const struct mystr> p_other)
{
  private_str_append_memchunk(p_str, p_other->p_buf, p_other->len);
}

void
str_append_text(_Ptr<struct mystr> p_str, _Nt_array_ptr<const char> p_src : count(0))
{
  vsf_sysutil_strlen_alt(p_src,s,len);
  private_str_append_memchunk(p_str, s, len);
}

void
str_append_char(_Ptr<struct mystr> p_str, char the_char)
{
  private_str_append_memchunk(p_str, &the_char, sizeof(the_char));
}

void
str_append_ulong(_Ptr<struct mystr> p_str, unsigned long the_ulong)
{
  str_append_text(p_str, vsf_sysutil_ulong_to_str(the_ulong));
}

void
str_append_filesize_t(_Ptr<struct mystr> p_str, filesize_t the_filesize)
{
  str_append_text(p_str, vsf_sysutil_filesize_t_to_str(the_filesize));
}

void
str_append_double(_Ptr<struct mystr> p_str, double the_double)
{
  str_append_text(p_str, vsf_sysutil_double_to_str(the_double));
}

void
str_upper(_Ptr<struct mystr> p_str)
{
  unsigned int i;
  for (i=0; i < p_str->len; i++)
  {
    p_str->p_buf[i] = (char) vsf_sysutil_toupper(p_str->p_buf[i]);
  }
}

void
str_rpad(_Ptr<struct mystr> p_str, const unsigned int min_width)
{
  unsigned int to_pad;
  if (p_str->len >= min_width)
  {
    return;
  }
  to_pad = min_width - p_str->len;
  while (to_pad--)
  {
    str_append_char(p_str, ' ');
  }
}

void
str_lpad(_Ptr<struct mystr> p_str, const unsigned int min_width)
{
  static struct mystr s_tmp_str;
  unsigned int to_pad;
  if (p_str->len >= min_width)
  {
    return;
  }
  to_pad = min_width - p_str->len;
  str_empty(&s_tmp_str);
  while (to_pad--)
  {
    str_append_char(&s_tmp_str, ' ');
  }
  str_append_str(&s_tmp_str, p_str);
  str_copy(p_str, &s_tmp_str);
}

void
str_replace_char(_Ptr<struct mystr> p_str, char from, char to)
{
  unsigned int i;
  for (i=0; i < p_str->len; i++)
  {
    if (p_str->p_buf[i] == from)
    {
      p_str->p_buf[i] = to;
    }
  }
}

void
str_replace_text(_Ptr<struct mystr> p_str, 
		 _Nt_array_ptr<const char> p_from : count(0),
		 _Nt_array_ptr<const char> p_to : count(0))
{
  static struct mystr s_lhs_chunk_str;
  static struct mystr s_rhs_chunk_str;
  unsigned int lhs_len;
  str_copy(&s_lhs_chunk_str, p_str);
  str_free(p_str);
  do
  {
    lhs_len = str_getlen(&s_lhs_chunk_str);
    str_split_text(&s_lhs_chunk_str, &s_rhs_chunk_str, (const char *)p_from);
    /* Copy lhs to destination */
    str_append_str(p_str, &s_lhs_chunk_str);
    /* If this was a 'hit', append the 'to' text */
    if (str_getlen(&s_lhs_chunk_str) < lhs_len)
    {
      str_append_text(p_str, p_to);
    }
    /* Current rhs becomes new lhs */
    str_copy(&s_lhs_chunk_str, &s_rhs_chunk_str);
  } while (!str_isempty(&s_lhs_chunk_str));
}

void
str_split_char(_Ptr<struct mystr> p_src, _Ptr<struct mystr> p_rhs, char c)
{
  /* Just use str_split_text */
  char ministr[2];
  ministr[0] = c;
  ministr[1] = '\0';
  str_split_text(p_src, p_rhs, ministr);
}

void
str_split_char_reverse(_Ptr<struct mystr> p_src, _Ptr<struct mystr> p_rhs, char c)
{
  /* Just use str_split_text_reverse */
  char ministr[2];
  ministr[0] = c;
  ministr[1] = '\0';
  str_split_text_reverse(p_src, p_rhs, ministr);
}

void
str_split_text(_Ptr<struct mystr> p_src, _Ptr<struct mystr> p_rhs, const char* p_text)
{
  str_split_text_common(p_src, p_rhs, p_text, 0);
}

void
str_split_text_reverse(_Ptr<struct mystr> p_src, _Ptr<struct mystr> p_rhs,
                       const char* p_text)
{
  str_split_text_common(p_src, p_rhs, p_text, 1);
}

static void
str_split_text_common(_Ptr<struct mystr> p_src, _Ptr<struct mystr> p_rhs,
                      const char* p_text, int is_reverse)
{
  struct str_locate_result locate_result;
  unsigned int indexx;
  unsigned int search_len = vsf_sysutil_strlen(p_text);
  if (is_reverse)
  {
    locate_result = str_locate_text_reverse(p_src, p_text);
  }
  else
  {
    locate_result = str_locate_text(p_src, p_text);
  }
  /* Not found? */
  if (!locate_result.found)
  {
    str_empty(p_rhs);
    return;
  }
  indexx = locate_result.index;
  if (indexx + search_len > p_src->len)
  {
    bug("indexx invalid in str_split_text");
  } 
  /* Build rhs */
  private_str_alloc_memchunk(p_rhs, p_src->p_buf + indexx + search_len,
                             p_src->len - indexx - search_len);
  /* Build lhs */
  str_trunc(p_src, indexx);
}

struct str_locate_result
str_locate_str(_Ptr<const struct mystr> p_str, _Ptr<const struct mystr> p_look_str)
{
  return str_locate_text(p_str, (const char *)str_getbuf(p_look_str));
}

struct str_locate_result
str_locate_str_reverse(_Ptr<const struct mystr> p_str,
                       _Ptr<const struct mystr> p_look_str)
{
  return str_locate_text_reverse(p_str, (const char *)str_getbuf(p_look_str));
}

struct str_locate_result
str_locate_char(_Ptr<const struct mystr> p_str, char look_char)
{
  char look_str[2];
  look_str[0] = look_char;
  look_str[1] = '\0';
  return str_locate_text(p_str, look_str);
}

struct str_locate_result
str_locate_chars(_Ptr<const struct mystr> p_str, const char* p_chars)
{
  struct str_locate_result retval;
  unsigned int num_chars = vsf_sysutil_strlen(p_chars);
  unsigned int i = 0;
  retval.found = 0;
  retval.char_found = 0;
  retval.index = 0;
  for (; i < p_str->len; ++i)
  {
    unsigned int j = 0;
    char this_char = p_str->p_buf[i];
    for (; j < num_chars; ++j)
    {
      if (p_chars[j] == this_char)
      {
        retval.found = 1;
        retval.index = i;
        retval.char_found = p_chars[j];
        return retval;
      }
    }
  }
  return retval;
}

struct str_locate_result
str_locate_text(_Ptr<const struct mystr> p_str, const char* p_text)
{
  struct str_locate_result retval;
  unsigned int i;
  unsigned int text_len = vsf_sysutil_strlen(p_text);
  retval.found = 0;
  retval.char_found = 0;
  retval.index = 0;
  if (text_len == 0 || text_len > p_str->len)
  {
    /* Not found */
    return retval;
  }
  for (i=0; i <= (p_str->len - text_len); i++)
  {
    if (vsf_sysutil_memcmp(p_str->p_buf + i, p_text, text_len) == 0)
    {
      retval.found = 1;
      retval.index = i;
      return retval;
    }
  }
  /* Not found */
  return retval;
}

struct str_locate_result
str_locate_text_reverse(_Ptr<const struct mystr> p_str, const char* p_text)
{
  struct str_locate_result retval;
  unsigned int i;
  unsigned int text_len = vsf_sysutil_strlen(p_text);
  retval.found = 0;
  retval.char_found = 0;
  retval.index = 0;
  if (text_len == 0 || text_len > p_str->len)
  {
    return retval;
  }
  i = p_str->len - text_len;
  /* Want to go through loop once even if i==0 */
  while (1)
  {
    if (vsf_sysutil_memcmp(p_str->p_buf + i, p_text, text_len) == 0)
    {
      retval.found = 1;
      retval.index = i;
      return retval;
    }
    if (i == 0)
    {
      break;
    }
    i--;
  }
  /* Not found */
  return retval;
}

void
str_left(_Ptr<const struct mystr> p_str, _Ptr<struct mystr> p_out, unsigned int chars)
{
  if (chars > p_str->len)
  {
    bug("chars invalid in str_left");
  }
  private_str_alloc_memchunk(p_out, p_str->p_buf, chars);
}

void
str_right(_Ptr<const struct mystr> p_str, _Ptr<struct mystr> p_out, unsigned int chars)
{
  unsigned int indexx = p_str->len - chars;
  if (chars > p_str->len)
  {
    bug("chars invalid in str_right");
  }
  private_str_alloc_memchunk(p_out, p_str->p_buf + indexx, chars);
}

void
str_mid_to_end(_Ptr<const struct mystr> p_str, _Ptr<struct mystr> p_out,
               unsigned int indexx)
{
  if (indexx > p_str->len)
  {
    bug("invalid indexx in str_mid_to_end");
  }
  private_str_alloc_memchunk(p_out, p_str->p_buf + indexx,
                             p_str->len - indexx);
}

char
str_get_char_at(_Ptr<const struct mystr> p_str, const unsigned int indexx)
{
  if (indexx >= p_str->len)
  {
    bug("bad indexx in str_get_char_at");
  }
  return p_str->p_buf[indexx];
}

int
str_contains_space(_Ptr<const struct mystr> p_str)
{
  unsigned int i;
  for (i=0; i < p_str->len; i++)
  {
    if (vsf_sysutil_isspace(p_str->p_buf[i]))
    {
      return 1;
    }
  }
  return 0;
}

int
str_all_space(_Ptr<const struct mystr> p_str)
{
  unsigned int i;
  for (i=0; i < p_str->len; i++)
  {
    if (!vsf_sysutil_isspace(p_str->p_buf[i]))
    {
      return 0;
    }
  }
  return 1;
}

int
str_contains_unprintable(_Ptr<const struct mystr> p_str)
{
  unsigned int i;
  for (i=0; i < p_str->len; i++)
  {
    if (!vsf_sysutil_isprint(p_str->p_buf[i]))
    {
      return 1;
    }
  }
  return 0;
}

int
str_atoi(_Ptr<const struct mystr> p_str)
{
  return vsf_sysutil_atoi((const char *)str_getbuf(p_str));
}

filesize_t
str_a_to_filesize_t(_Ptr<const struct mystr> p_str)
{
  return vsf_sysutil_a_to_filesize_t((const char *)str_getbuf(p_str));
}

unsigned int
str_octal_to_uint(_Ptr<const struct mystr> p_str)
{
  return vsf_sysutil_octal_to_uint((const char *)str_getbuf(p_str));
}

int
str_getline(_Ptr<const struct mystr> p_str, _Ptr<struct mystr> p_line_str,
            _Ptr<unsigned int> p_pos)
{
  unsigned int start_pos = *p_pos;
  unsigned int curr_pos = start_pos;
  unsigned int buf_len = str_getlen(p_str);
  _Nt_array_ptr<const char> p_buf_tmp : count(buf_len) = str_getbuf(p_str);
  _Array_ptr<const char> p_buf : count(buf_len) =
    _Assume_bounds_cast<_Array_ptr<const char>>(p_buf_tmp, buf_len); 
  unsigned int out_len;
  if (start_pos > buf_len)
  {
    bug("p_pos out of range in str_getline");
  }
  str_empty(p_line_str);
  if (start_pos == buf_len)
  {
    return 0;
  }
  while (curr_pos < buf_len && p_buf[curr_pos] != '\n')
  {
    curr_pos++;
    if (curr_pos == 0)
    {
      bug("integer overflow");
    }
  }
  out_len = curr_pos - start_pos;
  /* If we ended on a \n - skip it */
  if (curr_pos < buf_len && p_buf[curr_pos] == '\n')
  {
    curr_pos++;
    if (curr_pos == 0)
    {
      bug("integer overflow");
    }
  }
  private_str_alloc_memchunk(p_line_str, p_buf + start_pos, out_len);
  *p_pos = curr_pos;
  return 1;
}

int
str_contains_line(_Ptr<const struct mystr> p_str, _Ptr<const struct mystr> p_line_str)
{
  static struct mystr s_curr_line_str;
  unsigned int pos = 0;
  while (str_getline(p_str, &s_curr_line_str, &pos))
  {
    if (str_equal(&s_curr_line_str, p_line_str))
    {
      return 1;
    }
  }
  return 0;
}

void
str_replace_unprintable(_Ptr<struct mystr> p_str, char new_char)
{
  unsigned int i;
  for (i=0; i < p_str->len; i++)
  {
    if (!vsf_sysutil_isprint(p_str->p_buf[i]))
    {
      p_str->p_buf[i] = new_char;
    }
  }
}

