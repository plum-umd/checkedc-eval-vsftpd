#ifndef VSFTP_STR_H
#define VSFTP_STR_H

/* TODO - document these functions ;-) */

#ifndef VSF_FILESIZE_H
#include "filesize.h"
#endif

struct mystr
{
  char* PRIVATE_HANDS_OFF_p_buf;
  /* Internally, EXCLUDES trailing null */
  unsigned int PRIVATE_HANDS_OFF_len;
  unsigned int PRIVATE_HANDS_OFF_alloc_bytes;
};

#define INIT_MYSTR \
  { (void*)0, 0, 0 }

#ifdef VSFTP_STRING_HELPER
#define str_alloc_memchunk private_str_alloc_memchunk
#endif
void private_str_alloc_memchunk(_Ptr<struct mystr> p_str, const char *p_src : itype(_Array_ptr<const char>) byte_count(len), unsigned int len);

void str_alloc_text(_Ptr<struct mystr> p_str, const char *p_src : itype(_Nt_array_ptr<const char>));
/* NOTE: String buffer data does NOT include terminating character */
void str_alloc_alt_term(_Ptr<struct mystr> p_str, const char *p_src : itype(_Array_ptr<const char>), char term);
void str_alloc_ulong(_Ptr<struct mystr> p_str, unsigned long the_long);
void str_alloc_filesize_t(_Ptr<struct mystr> p_str, filesize_t the_filesize);
void str_copy(_Ptr<struct mystr> p_dest, _Ptr<const struct mystr> p_src);
const char *str_strdup(_Ptr<const struct mystr> p_str) : itype(_Nt_array_ptr<const char>);
void str_empty(_Ptr<struct mystr> p_str);
void str_free(_Ptr<struct mystr> p_str);
void str_trunc(_Ptr<struct mystr> p_str, unsigned int trunc_len);
void str_reserve(_Ptr<struct mystr> p_str, unsigned int res_len);

int str_isempty(_Ptr<const struct mystr> p_str);
unsigned int str_getlen(_Ptr<const struct mystr> p_str);
const char* str_getbuf(_Ptr<const struct mystr> p_str);

int str_strcmp(_Ptr<const struct mystr> p_str1, _Ptr<const struct mystr> p_str2);
int str_equal(_Ptr<const struct mystr> p_str1, _Ptr<const struct mystr> p_str2);
int str_equal_text(_Ptr<const struct mystr> p_str, _Nt_array_ptr<const char> p_text);

void str_append_str(_Ptr<struct mystr> p_str, _Ptr<const struct mystr> p_other);
void str_append_text(_Ptr<struct mystr> p_str, const char *p_src : itype(_Nt_array_ptr<const char>));
void str_append_ulong(_Ptr<struct mystr> p_str, unsigned long the_ulong);
void str_append_filesize_t(_Ptr<struct mystr> p_str, filesize_t the_filesize);
void str_append_char(_Ptr<struct mystr> p_str, char the_char);
void str_append_double(_Ptr<struct mystr> p_str, double the_double);

void str_upper(_Ptr<struct mystr> p_str);
void str_rpad(_Ptr<struct mystr> p_str, const unsigned int min_width);
void str_lpad(_Ptr<struct mystr> p_str, const unsigned int min_width);
void str_replace_char(_Ptr<struct mystr> p_str, char from, char to);
void str_replace_text(_Ptr<struct mystr> p_str, const char *p_from : itype(_Nt_array_ptr<const char>) count(1), const char *p_to : itype(_Nt_array_ptr<const char>));

void str_split_char(_Ptr<struct mystr> p_src, _Ptr<struct mystr> p_rhs, char c);
void str_split_char_reverse(_Ptr<struct mystr> p_src, _Ptr<struct mystr> p_rhs, char c);
void str_split_text(_Ptr<struct mystr> p_src, _Ptr<struct mystr> p_rhs, _Nt_array_ptr<const char> p_text : count(1));
void str_split_text_reverse(_Ptr<struct mystr> p_src, _Ptr<struct mystr> p_rhs, _Nt_array_ptr<const char> p_text : count(1));

struct str_locate_result
{
  int found;
  unsigned int index;
  char char_found;
};

struct str_locate_result str_locate_char(_Ptr<const struct mystr> p_str, char look_char);
struct str_locate_result str_locate_str(_Ptr<const struct mystr> p_str, _Ptr<const struct mystr> p_look_str);
struct str_locate_result str_locate_str_reverse(_Ptr<const struct mystr> p_str, _Ptr<const struct mystr> p_look_str);
struct str_locate_result str_locate_text(_Ptr<const struct mystr> p_str, const char *p_text : itype(_Nt_array_ptr<const char>) count(1));
struct str_locate_result str_locate_text_reverse(_Ptr<const struct mystr> p_str, const char *p_text : itype(_Nt_array_ptr<const char>) count(1));
struct str_locate_result str_locate_chars(_Ptr<const struct mystr> p_str, _Nt_array_ptr<const char> p_chars);

void str_left(_Ptr<const struct mystr> p_str, _Ptr<struct mystr> p_out, unsigned int chars);
void str_right(_Ptr<const struct mystr> p_str, _Ptr<struct mystr> p_out, unsigned int chars);
void str_mid_to_end(_Ptr<const struct mystr> p_str, _Ptr<struct mystr> p_out, unsigned int indexx);

char str_get_char_at(_Ptr<const struct mystr> p_str, const unsigned int indexx);
int str_contains_space(_Ptr<const struct mystr> p_str);
int str_all_space(_Ptr<const struct mystr> p_str);
int str_contains_unprintable(_Ptr<const struct mystr> p_str);
void str_replace_unprintable(_Ptr<struct mystr> p_str, char new_char);
int str_atoi(_Ptr<const struct mystr> p_str);
filesize_t str_a_to_filesize_t(_Ptr<const struct mystr> p_str);
unsigned int str_octal_to_uint(_Ptr<const struct mystr> p_str);

/* PURPOSE: Extract a line of text (delimited by \n or EOF) from a string
 * buffer, starting at character position 'p_pos'. The extracted line will
 * not contain the '\n' terminator.
 *
 * RETURNS: 0 if no more lines are available, 1 otherwise.
 * The extracted text line is stored in 'p_line_str', which is
 * emptied if there are no more lines. 'p_pos' is updated to point to the
 * first character after the end of the line just extracted.
 */
int str_getline(_Ptr<const struct mystr> p_str, _Ptr<struct mystr> p_line_str, _Ptr<unsigned int> p_pos);

/* PURPOSE: Detect whether or not a string buffer contains a specific line
 * of text (delimited by \n or EOF).
 *
 * RETURNS: 1 if there is a matching line, 0 otherwise.
 */
int str_contains_line(_Ptr<const struct mystr> p_str, _Ptr<const struct mystr> p_line_str);

#endif /* VSFTP_STR_H */

