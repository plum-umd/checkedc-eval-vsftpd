#ifndef VSFTP_HASH_H
#define VSFTP_HASH_H

struct hash;

typedef unsigned int (*hashfunc_t)(unsigned int, void*);

_Ptr<struct hash> hash_alloc(unsigned int buckets, unsigned int key_size, unsigned int value_size, _Ptr<unsigned int (unsigned int , void *)> hash_func);
void* hash_lookup_entry(_Ptr<struct hash> p_hash, void* p_key);
void hash_add_entry(_Ptr<struct hash> p_hash, void* p_key, void* p_value);
void hash_free_entry(_Ptr<struct hash> p_hash, void* p_key);

#endif /* VSFTP_HASH_H */

