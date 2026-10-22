/*
 * A337042  a(n) = exp(-1/6) * Sum_{k>=0} (6*k - 1)^n / (6^k * k!).
 * https://oeis.org/A337042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A337042
   a(0) = 1
   a(1) = 0
   a(n) = Sum_{k=1..n-1} binomial(n-1,k) * 6^k * a(n-k-1)   for n ≥ 2
   Implementation uses a memoised dynamic programming approach. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* provides: seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* initialise cache with a(0) */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* if the requested term is already known, return it */
    if (n < (idx_t)cache_len) {
        return cache[(size_t)n];
    }

    /* ensure the cache can hold the requested term */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (size_t)(n + 1)) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute all intermediate terms up to n */
    for (idx_t m = (idx_t)cache_len; m <= n; ++m) {
        seq_t val;
        if (m == 0) {
            val = (seq_t)1;
        } else if (m == 1) {
            val = (seq_t)0;
        } else {
            seq_t sum  = (seq_t)0;
            seq_t pow6 = (seq_t)1;   /* 6^0 */
            for (idx_t k = 1; k <= m - 1; ++k) {
                /* 6^k */
                pow6 = pow6 * (seq_t)6;

                /* binomial coefficient C(m-1, k) */
                seq_t comb = binomial(m - 1, k);

                /* term = C(m-1,k) * 6^k * a[m-1-k] */
                seq_t term = comb * pow6;
                term = term * cache[(size_t)(m - 1 - k)];
                sum += term;
            }
            val = sum;
        }
        cache[(size_t)m] = val;
        if ((size_t)m + 1 > cache_len) {
            cache_len = (size_t)m + 1;
        }
    }

    return cache[(size_t)n];
}
