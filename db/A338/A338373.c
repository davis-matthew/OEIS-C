/*
 * A338373  Numbers k such that bigomega(2*k + 1) >= 4.
 * https://oeis.org/A338373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 9572, u32 n <= 9572, u64 n <= 9572,
 *            u128 n <= 9572, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A338373: a(n) is the nth integer k for which
   bigomega(2*k + 1) >= 4 (offset 1).  The implementation
   uses lazy memoisation: terms are generated on demand by
   testing successive candidates. */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/bigomega.h"

static seq_t *cache    = NULL;   /* cache of found k's          */
static size_t cache_len = 0;     /* number of cached terms      */
static size_t cache_cap = 0;     /* current capacity of cache   */
static idx_t  next_k    = 1;     /* next k to test (>=1)        */

/* Ensure 'cache' can hold at least 'needed' elements.  */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < needed) newcap <<= 1;
    /* cast the result of realloc to the appropriate pointer type
       for both C and C++  */
    seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();           /* out of memory              */
    cache = tmp;
    cache_cap = newcap;
}

/* Return the n‑th term (n ≥ 1).  The argument n may be
   called in any order; results are memoised.            */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* dummy value for n = 0 */

    /* if already cached, return it */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* generate terms until we have at least n terms */
    while (cache_len < (size_t)n) {
        idx_t k     = next_k++;
        idx_t odd   = k * 2 + 1;          /* 2*k + 1   */
        seq_t omega = bigomega(odd);      /* Ω(odd)    */

        if (omega >= (seq_t)4) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)k;   /* store k */
        }
    }
    return cache[n - 1];
}
