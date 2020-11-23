#ifndef VSF_STRLIST_H
#define VSF_STRLIST_H

/* Forward declarations */
struct mystr;
struct mystr_list_node;

struct mystr_list
{
  unsigned int PRIVATE_HANDS_OFF_alloc_len;
  unsigned int PRIVATE_HANDS_OFF_list_len;
  _Array_ptr<struct mystr_list_node> PRIVATE_HANDS_OFF_p_nodes : count(PRIVATE_HANDS_OFF_alloc_len);
};

#define INIT_STRLIST \
  { 0, 0, (void*)0 }

void str_list_free(_Ptr<struct mystr_list> p_list);

void str_list_add(struct mystr_list *p_list : itype(_Ptr<struct mystr_list>), _Ptr<const struct mystr> p_str, _Ptr<const struct mystr> p_sort_key_str);
void str_list_sort(_Ptr<struct mystr_list> p_list, int reverse);

unsigned int str_list_get_length(const struct mystr_list *p_list : itype(_Ptr<const struct mystr_list>));
int str_list_contains_str(const struct mystr_list *p_list : itype(_Ptr<const struct mystr_list>), _Ptr<const struct mystr> p_str);

_Ptr<const struct mystr> str_list_get_pstr(_Ptr<const struct mystr_list> p_list, unsigned int indexx);

#endif /* VSF_STRLIST_H */

