#ifndef VSF_SECBUF_H
#define VSF_SECBUF_H

#pragma BOUNDS_CHECKED ON

/* vsf_secbuf_alloc()
 * PURPOSE
 * Allocate a "secure buffer". A secure buffer is one which will attempt to
 * catch out of bounds accesses by crashing the program (rather than
 * corrupting memory). It works by using UNIX memory protection. It isn't
 * foolproof.
 * PARAMETERS
 * p_ptr        - pointer to a pointer which is to contain the secure buffer.
 *                Any previous buffer pointed to is freed.
 * size         - size in bytes required for the secure buffer.
 */
_Nt_array_ptr<char> vsf_secbuf_alloc(_Array_ptr<char> ptr, unsigned int size) : count(size);

/* vsf_secbuf_free()
 * PURPOSE
 * Frees a "secure buffer".
 * PARAMETERS
 * p_ptr        - pointer to a pointer containing the buffer to be freed. The
 *                buffer pointer is nullified by this call.
 */
void vsf_secbuf_free(_Array_ptr<char> p_ptr);

#pragma BOUNDS_CHECKED OFF

#endif /* VSF_SECBUF_H */

