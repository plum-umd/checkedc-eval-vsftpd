#ifndef VSFTP_HASH_H
#define VSFTP_HASH_H

#pragma BOUNDS_CHECKED ON

struct hash;

/* typedef unsigned int (*hashfunc_t)(unsigned int, void*); */
typedef _Ptr<unsigned int (unsigned int, _Ptr<void>)> hashfunc_t;

_Ptr<struct hash> hash_alloc(unsigned int buckets, unsigned int key_size,
                        unsigned int value_size, hashfunc_t hash_func);
_Ptr<void> hash_lookup_entry(_Ptr<struct hash> p_hash, _Ptr<void> p_key);
void hash_add_entry(_Ptr<struct hash> p_hash, _Ptr<void> p_key, _Ptr<void> p_value);
void hash_free_entry(_Ptr<struct hash> p_hash, _Ptr<void> p_key);

#pragma BOUNDS_CHECKED OFF

#endif /* VSFTP_HASH_H */

