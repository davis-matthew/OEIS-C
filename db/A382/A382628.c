/*
 * A382628  Centered hexagonal numbers that are sphenic numbers.
 * https://oeis.org/A382628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A382628: the n‑th centered hexagonal number that is a sphenic
 * number.  The centered hexagonal numbers are C(k)=3*k*(k-1)+1 (k≥1).
 * A number is sphenic iff it has exactly three distinct prime factors,
 * each with exponent 1.  This implementation tests successive C(k)
 * values, keeps a static cache, and answers each query in O(1).  For
 * factorisation a 64‑bit routine is used – all terms needed for n≤10000
 * fit comfortably in 64 bits. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n,uint64_t *pr,uint64_t *ex,size_t *cnt); */

/* Test whether a 64‑bit integer is a sphenic number. */
static bool is_sphenic_uint64(uint64_t x)
{
    if (x < 30) return false;                           /* smallest sphenic is 2*3*5=30 */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(x, primes, exps, &cnt);
    if (cnt != 3) return false;
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] != 1) return false;
    return true;
}

/* Return the n‑th centered hexagonal sphenic number (n≥1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                     /* undefined for 0, avoid UB */

    /* Static cache to store found terms. */
    static seq_t *cache      = NULL;                 /* cache array */
    static size_t cache_cap  = 0;                    /* allocated capacity */
    static size_t cache_len  = 0;                    /* number of cached terms */
    static idx_t  next_k     = 1;                    /* next k to test */

    /* Initialise cache once. */
    if (cache == NULL) {
        cache_cap = 128;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                         /* memory allocation failure */
    }

    /* Generate centered hexagonal numbers until we have n terms. */
    while (cache_len < n) {
        /* C(k) = 3*k*(k-1)+1.  Use 128‑bit maths to avoid overflow. */
        unsigned __int128 t = (unsigned __int128)next_k * (next_k - 1ULL);
        seq_t cand = (seq_t)(t * 3 + 1);

        /* Test cand for sphenic property – cast to 64‑bit for factorisation. */
        uint64_t cand_u64 = (uint64_t)cand;
        if (cand_u64 <= 0xffffffffffffffull) {
            if (is_sphenic_uint64(cand_u64)) {
                /* Grow cache if necessary. */
                if (cache_len == cache_cap) {
                    cache_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                }
                cache[cache_len++] = cand;
            }
        }

        ++next_k;
    }

    return cache[n - 1];
}
