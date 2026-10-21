/*
 * A242331  Numbers k such that k^2 + 3 is a semiprime.
 * https://oeis.org/A242331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th integer k such that k^2+3 is a semiprime.
   It searches k in increasing order, tests k^2+3 with bigomega, and
   caches the resulting k values. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Values for n = 0 are unused
       but are returned as 0 to keep the function well defined. */
    if (n == 0)
        return 0;

    /* Static cache of previously found terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of elements in cache   */
    static size_t cache_cap  = 0;   /* allocated capacity             */
    static idx_t  last_k     = 0;   /* last k checked                 */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        ++last_k;                                    /* next candidate k */
        uint64_t m = (uint64_t)last_k * (uint64_t)last_k + 3ULL; /* k^2 + 3 */

        /* bigomega counts prime factors with multiplicity. */
        if (bigomega((idx_t)m) == (seq_t)2) {         /* m is semiprime    */
            /* Grow cache if necessary. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = (seq_t)last_k;       /* store k           */
        }
    }

    return cache[n - 1];
}
