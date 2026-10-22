/*
 * A332456  Numbers k such that sigma(sigma(k)) is congruent to 6 modulo 8.
 * https://oeis.org/A332456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 980, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th positive integer k such that σ(σ(k)) ≡ 6 (mod 8).
   It searches sequentially and caches found terms. */

/* Include utilities and standard allocation helpers */
#include <stdlib.h>
#include "../utils/sigma_of.h"

/* external types:
   - seq_t   : unsigned integer type of arbitrary precision
   - idx_t   : unsigned 64‑bit integer (uint64_t) */

seq_t a(idx_t n)
{
    /* static cache holding the found terms */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t  last_k     = 0;   /* largest k already processed */

    /* Generate terms until we have at least n of them */
    while (cache_size < n) {
        ++last_k;   /* candidate k */

        /* compute σ(k) */
        seq_t sigma_k = sigma_of(last_k);

        /* sigma_k may exceed IDX_T range for very large k;
           in practice for n ≤ 10000 it always fits in uint64_t. */
        if (sigma_k > (seq_t)(UINT64_MAX)) {
            /* skip this k if σ(k) is out of range of sigma_of */
            continue;
        }

        /* compute σ(σ(k)) */
        seq_t sigma_sigma = sigma_of((idx_t)sigma_k);

        /* check the congruence condition */
        if ((sigma_sigma & (seq_t)7) == (seq_t)6) {   /* sigma_sigma % 8 == 6 */
            /* grow the cache if needed */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failure – return 0 as a sentinel */
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)last_k;   /* store qualifying k */
        }
    }

    return cache[n - 1];
}
