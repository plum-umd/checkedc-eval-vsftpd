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
  void* p_key;
  void* p_value;
  _Ptr<struct hash_node> p_prev;
  _Ptr<struct hash_node> p_next;
};

struct hash
{
  unsigned int buckets;
  unsigned int key_size;
  unsigned int value_size;
  hashfunc_t hash_func;
  _Ptr<struct hash_node>* p_nodes;
};

/* Internal functions */
_Ptr<struct hash_node>* hash_get_bucket(_Ptr<struct hash> p_hash, void* p_key);
_Ptr<struct hash_node> hash_get_node_by_key(_Ptr<struct hash> p_hash, void* p_key);

_Ptr<struct hash>
hash_alloc(unsigned int buckets, unsigned int key_size,
           unsigned int value_size, hashfunc_t hash_func)
{
  unsigned int size;
  _Ptr<struct hash> p_hash = _Assume_bounds_cast<_Ptr<struct hash>>(vsf_sysutil_malloc(sizeof(*p_hash)));
  p_hash->buckets = buckets;
  p_hash->key_size = key_size;
  p_hash->value_size = value_size;
  p_hash->hash_func = hash_func;
  size = (unsigned int) sizeof(_Ptr<struct hash_node>) * buckets;
  p_hash->p_nodes = vsf_sysutil_malloc(size);
  vsf_sysutil_memclr(p_hash->p_nodes, size);
  return p_hash;
}

void*
hash_lookup_entry(_Ptr<struct hash> p_hash, void* p_key)
{
  _Ptr<struct hash_node> p_node = hash_get_node_by_key(p_hash, p_key);
  if (!p_node)
  {
    return 0;
  }
  return p_node->p_value;
}

void
hash_add_entry(_Ptr<struct hash> p_hash, void* p_key, void* p_value)
{
  _Ptr<struct hash_node>* p_bucket;
  _Ptr<struct hash_node> p_new_node = 0;
  if (hash_lookup_entry(p_hash, p_key))
  {
    bug("duplicate hash key");
  }
  p_bucket = hash_get_bucket(p_hash, p_key);
  p_new_node = _Assume_bounds_cast<_Ptr<struct hash_node>>(vsf_sysutil_malloc(sizeof(*p_new_node)));
  p_new_node->p_prev = 0;
  p_new_node->p_next = 0;
  p_new_node->p_key = vsf_sysutil_malloc(p_hash->key_size);
  vsf_sysutil_memcpy(p_new_node->p_key, p_key, p_hash->key_size);
  p_new_node->p_value = vsf_sysutil_malloc(p_hash->value_size);
  vsf_sysutil_memcpy(p_new_node->p_value, p_value, p_hash->value_size);

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

void
hash_free_entry(_Ptr<struct hash> p_hash, void* p_key)
{
  _Ptr<struct hash_node> p_node = hash_get_node_by_key(p_hash, p_key);
  if (!p_node)
  {
    bug("hash node not found");
  }
  vsf_sysutil_free(p_node->p_key);
  vsf_sysutil_free(p_node->p_value);

  if (p_node->p_prev)
  {
    p_node->p_prev->p_next = p_node->p_next;
  }
  else
  {
    _Ptr<struct hash_node>* p_bucket = hash_get_bucket(p_hash, p_key);
    *p_bucket = p_node->p_next;
  }
  if (p_node->p_next)
  {
    p_node->p_next->p_prev = p_node->p_prev;
  }

  vsf_sysutil_free(p_node);
}

_Ptr<struct hash_node>*
hash_get_bucket(_Ptr<struct hash> p_hash, void* p_key)
{
  unsigned int bucket = (*p_hash->hash_func)(p_hash->buckets, p_key);
  if (bucket >= p_hash->buckets)
  {
    bug("bad bucket lookup");
  }
  return &(p_hash->p_nodes[bucket]);
}

_Ptr<struct hash_node>
hash_get_node_by_key(_Ptr<struct hash> p_hash, void* p_key)
{
  _Ptr<struct hash_node>* p_bucket = hash_get_bucket(p_hash, p_key);
  _Ptr<struct hash_node> p_node = *p_bucket;
  if (!p_node)
  {
    return p_node;
  }
  while (p_node != 0 &&
         vsf_sysutil_memcmp(p_key, p_node->p_key, p_hash->key_size) != 0)
  {
    p_node = p_node->p_next;
  }
  return p_node;
}

