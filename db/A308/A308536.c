/*
 * A308536  Expansion of e.g.f. exp(1 - exp(2*x)).
 * https://oeis.org/A308536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{k=1..n} 2^k * binomial(n-1,k-1) * a(n-k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 14, i64 n <= 22, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of elements currently allocated */

    /* initialise cache[0] on first call */
    if (cache_cap == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache_cap = 1;
    }

    /* grow array if necessary */
    size_t need = (size_t)n + 1;                 /* elements needed up to n inclusive */
    size_t old_cap = cache_cap;
    if (need > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = need;
    }

    /* compute missing terms starting from the first uncomputed index */
    size_t start = (old_cap == 0) ? 1 : old_cap; /* cache[0] is already set */
    for (size_t i = start; i <= (size_t)n; ++i) {
        idx_t m = (idx_t)(i - 1);
        seq_t C = (seq_t)1;          /* binomial(m,0) */
        seq_t pow2k = (seq_t)2;      /* 2^1 */
        __int128 sum128 = 0;

        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = cache[i - k];
            /* compute 2^k * binom(m,k-1) * a[i-k] using 128‑bit intermediate */
            __int128 prod = (__int128)pow2k * (__int128)C * (__int128)term;
            sum128 += prod;

            if (k < i) {
                /* update binomial coefficient to binom(m,k) */
                C = C * (m - (k - 1)) / k;
                /* advance power of two */
                pow2k <<= 1;
            }
        }

        cache[i] = (seq_t)(-sum128);
    }

    return cache[n];
}
