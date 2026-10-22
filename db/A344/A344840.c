/*
 * A344840  a(0) = 1; a(n) = 5 * Sum_{k=1..n} binomial(n,k) * a(k-1).
 * https://oeis.org/A344840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 5 * Σ_{k=1..n} binomial(n,k) * a(k-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t computed = 0;   /* number of values already stored (next to compute) */

    /* base case */
    if (n == 0)
        return (seq_t)1;

    /* make sure the cache can hold up to index n */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise the first element if this is the first call */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;
    }

    /* compute all missing entries up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;   /* binomial(i,0) */

        /* k runs from 1 to i; b‌تواند вычислять binomial(i,k) iteratively */
        for (idx_t k = 1; k <= i; ++k) {
            /* update binomial coefficient: C(i,k) = C(i,k-1) * (i-k+1) / k */
            binom = (binom * (seq_t)(i - k + 1)) / (seq_t)k;
            sum += binom * cache[k - 1];
        }

        cache[i] = 5 * sum;
    }

    computed = n + 1;
    return cache[n];
}
