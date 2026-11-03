/*
 * A274359  Numbers k such that k and k+1 both have 16 divisors.
 * https://oeis.org/A274359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 402, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9699 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): numbers k such that d(k)=d(k+1)=16 (where d(n) is the divisor count). */
/* The function searches sequentially, memoising found values. */
/* It is safe for any external linkage, any seq_t size, and any idx_t. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t len = 0;      /* number of stored terms   */
    static size_t cap = 0;      /* capacity of the cache    */
    static uint64_t cur = 1;    /* next candidate to test   */

    /* extend the cache until we have a(n) available */
    while (len < n) {
        uint64_t k   = cur;
        uint64_t k1  = cur + 1;

        /* k and k+1 must both have exactly 16 divisors */
        if (num_divisors(k) == 16 && num_divisors(k1) == 16) {
            /* grow cache if necessary */
            if (len + 1 > cap) {
                size_t newcap = (cap > 0) ? cap * 2 : 16;
                if (len + 1 > newcap) newcap = len + 1;
                ensure_capacity(newcap, &cap, OEISC_VPP(&cache), sizeof(seq_t));
            }
            cache[len++] = (seq_t)k; /* store the term */
        }
        ++cur; /* next integer to test */
    }

    return cache[n - 1];
}
