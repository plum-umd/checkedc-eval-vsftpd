/*
 * Part of Very Secure FTPd
 * Licence: GPL v2
 * Author: Chris Evans
 * hash.c
 *
 * Routines to handle simple hash table lookups and modifications.
 */

#include "hash.h"
#include "sysutil.h"
#include "utility.h"

struct hash_node
{
  _Array_ptr<void> p_key;
  _Array_ptr<void> p_value;
  struct hash_node* p_prev : itype(_Ptr<struct hash_node>);
  struct hash_node* p_next : itype(_Ptr<struct hash_node>);
};

struct hash
{
  unsigned int buckets;
  unsigned int key_size;
  unsigned int value_size;
  _Ptr<unsigned int (unsigned int , void *)> hash_func;
  _Array_ptr<struct hash_node *> p_nodes;
};

/* Internal functions */
_Itype_for_any(K) struct hash_node** hash_get_bucket(_Ptr<struct hash> p_hash, void* p_key : itype(_Ptr<K>)) : itype(_Ptr<_Ptr<struct hash_node>>);
_Itype_for_any(K) struct hash_node* hash_get_node_by_key(_Ptr<struct hash> p_hash, void* p_key : itype(_Ptr<K>)) : itype(_Ptr<struct hash_node>);

_Ptr<struct hash> 
hash_alloc(unsigned int buckets, unsigned int key_size, unsigned int value_size, _Ptr<unsigned int (unsigned int , void *)> hash_func)
{
  unsigned int size;
  _Ptr<struct hash> p_hash = vsf_sysutil_malloc<struct hash>(sizeof(*p_hash));
  p_hash->buckets = buckets;
  p_hash->key_size = key_size;
  p_hash->value_size = value_size;
  p_hash->hash_func = hash_func;
  size = (unsigned int) sizeof(struct hash_node*) * buckets;
  p_hash->p_nodes = vsf_sysutil_malloc<struct hash_node *>(size);
  _Array_ptr<struct hash_node *> temp : byte_count(size) = _Assume_bounds_cast<_Array_ptr<struct hash_node *>>(p_hash->p_nodes, byte_count(size));
  vsf_sysutil_memclr<struct hash_node *>(temp, size);
  return p_hash;
}

_Itype_for_any(K, V) void*
hash_lookup_entry(struct hash *p_hash : itype(_Ptr<struct hash>), void* p_key : itype(_Ptr<K>)) : itype(_Ptr<V>)
{
  struct hash_node* p_node = hash_get_node_by_key<void>(p_hash, p_key);
  if (!p_node)
  {
    return p_node;
  }
  return  _Assume_bounds_cast<_Ptr<V>>(p_node->p_value);
}

_Itype_for_any(K, V) void
hash_add_entry(struct hash *p_hash : itype(_Ptr<struct hash>), void* p_key : itype(_Ptr<K>), void* p_value : itype(_Ptr<V>))
{
  _Ptr<_Ptr<struct hash_node>> p_bucket = ((void *)0);
  _Ptr<struct hash_node> p_new_node = ((void *)0);
  if (hash_lookup_entry<void, void>(p_hash, p_key))
  {
    bug("duplicate hash key");
  }
  p_bucket = (_Ptr<_Ptr<struct hash_node>>) hash_get_bucket<void>(p_hash, p_key);
  p_new_node = vsf_sysutil_malloc<struct hash_node>(sizeof(*p_new_node));
  p_new_node->p_prev = 0;
  p_new_node->p_next = 0;
  _Ptr<K> new_key = vsf_sysutil_malloc<K>(p_hash->key_size);
  p_new_node->p_key = new_key;
  vsf_sysutil_memcpy<K>(((void *)new_key), ((const void *)p_key), p_hash->key_size);
  _Ptr<V> new_val = vsf_sysutil_malloc<V>(p_hash->value_size);
  p_new_node->p_value = new_val;
  vsf_sysutil_memcpy<V>(((void *)new_val), ((const void *)p_value), p_hash->value_size);

  if (!*p_bucket)
  {
    *p_bucket = p_new_node;    
  }
  else
  {
    p_new_node->p_next = *p_bucket;
    (*p_bucket)->p_prev = p_new_node;
    *p_bucket = p_new_node;
  }
}

_Itype_for_any(K) void
hash_free_entry(struct hash *p_hash : itype(_Ptr<struct hash>), void* p_key : itype(_Ptr<K>))
{
  _Ptr<struct hash_node> p_node = hash_get_node_by_key<K>(p_hash, p_key);
  if (!p_node)
  {
    bug("hash node not found");
  }
  _Ptr<K> old_key = _Assume_bounds_cast<_Ptr<K>>(p_node->p_key);
  vsf_sysutil_free<K>(old_key);
  vsf_sysutil_free<void>((_Ptr<void>) p_node->p_value);

  if (p_node->p_prev)
  {
    p_node->p_prev->p_next = p_node->p_next;
  }
  else
  {
    _Ptr<_Ptr<struct hash_node>> p_bucket = (_Ptr<_Ptr<struct hash_node>>) hash_get_bucket<K>(p_hash, p_key);
    *p_bucket = p_node->p_next;
  }
  if (p_node->p_next)
  {
    p_node->p_next->p_prev = p_node->p_prev;
  }

  vsf_sysutil_free<struct hash_node>(p_node);
}

_Itype_for_any(K) struct hash_node**
hash_get_bucket(_Ptr<struct hash> p_hash, void* p_key : itype(_Ptr<K>)) : itype(_Ptr<_Ptr<struct hash_node>>)
{
  unsigned int bucket = (*p_hash->hash_func)(p_hash->buckets, p_key);
  if (bucket >= p_hash->buckets)
  {
    bug("bad bucket lookup");
  }
  return _Assume_bounds_cast<_Ptr<_Ptr<struct hash_node>>>(p_hash->p_nodes + bucket);
}

_Itype_for_any(K) struct hash_node*
hash_get_node_by_key(_Ptr<struct hash> p_hash, void* p_key : itype(_Ptr<K>)) : itype(_Ptr<struct hash_node>)
{
  _Ptr<_Ptr<struct hash_node>> p_bucket = (_Ptr<_Ptr<struct hash_node>>) hash_get_bucket<void>(p_hash, p_key);
  _Ptr<struct hash_node> p_node = *p_bucket;
  if (!p_node)
  {
    return p_node;
  }
  while (p_node != 0 &&
         vsf_sysutil_memcmp<void>(((const void *)p_key), _Assume_bounds_cast<_Array_ptr<const void>>(p_node->p_key, byte_count(p_hash->key_size)), p_hash->key_size) != 0)
  {
    p_node = p_node->p_next;
  }
  return p_node;
}

