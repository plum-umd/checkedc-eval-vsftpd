#ifndef VSFTP_HASH_H
#define VSFTP_HASH_H

struct hash;

typedef unsigned int (*hashfunc_t)(unsigned int, void*);

_Ptr<struct hash>  hash_alloc(unsigned int buckets, unsigned int key_size, unsigned int value_size, _Ptr<unsigned int (unsigned int , void *) > hash_func);
_Itype_for_any(K, V) void* hash_lookup_entry(struct hash *p_hash : itype(_Ptr<struct hash>), void* p_key : itype(_Ptr<K>)) : itype(_Ptr<V>);
_Itype_for_any(K, V) void hash_add_entry(struct hash *p_hash : itype(_Ptr<struct hash>), void* p_key : itype(_Ptr<K>), void* p_value : itype(_Ptr<V>));
_Itype_for_any(K) void hash_free_entry(struct hash *p_hash : itype(_Ptr<struct hash>), void* p_key : itype(_Ptr<K>));

#endif /* VSFTP_HASH_H */

