/*
 * A337591  a(0) = 1; a(n) = (1/n) * Sum_{k=1..n} binomial(n,k)^2 * k^3 *
 *          a(n-k).
 * https://oeis.org/A337591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{k=1..n} binomial(n,k)^2 * k^3 * a(n-k) with
 *            a(0) = 1. Uses memoised dynamic array cache
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache holding computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* grow cache to be able to store a[n] */
    if ((size_t)n + 1 > cache_size) {
        size_t new_cap = cache_size ? cache_size * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();     /* out of memory */
        cache = tmp;
        cache_size = new_cap;
    }

    /* initialise base case */
    if (cache[0] == 0) cache[0] = 1;
    if (n == 0) return cache[0];

    /* compute all values up to n (if not already cached) */
    for (idx_t m = 1; m <= n; ++m) {
        if (cache[m] != 0) continue;   /* already computed */

        seq_t sum = 0;
        seq_t bin = 1;                 /* C(m,0) */

        for (idx_t k = 1; k <= m; ++k) {
            /* C(m,k) = C(m,k-1) * (m-k+1) / k  (exact division) */
            bin = bin * ((seq_t)m - k + 1) / (seq_t)k;

            /* (bin^2) * k^3 * a(m-k) */
            seq_t term = bin * bin;                    /* bin^2 */
            term *= (seq_t)k * (seq_t)k * (seq_t)k;     /* k^3 */
            term *= cache[m - k];                      /* a(m-k) */

            sum += term;
        }

        cache[m] = sum / (seq_t)m;   /* division is exact by definition */
    }

    return cache[n];
}
