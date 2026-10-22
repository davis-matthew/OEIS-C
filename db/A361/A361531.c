/*
 * A361531  Expansion of e.g.f. exp(1 - exp(x) + x^3/6).
 * https://oeis.org/A361531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = binomial(n-1,2) * a(n-3) - Sum_{k=1..n} binomial(n-1,k-1)
 *            * a(n-k) a(0) = 1, a(1) = -1, a(2) = 0
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 19, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Extend the cache if necessary and compute missing values */
    if ((idx_t)cache_size <= n) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* Compute all terms from the current size up to n */
        for (size_t idx = cache_size; idx <= n; ++idx) {
            idx_t i = (idx_t)idx;
            if (i == 0) {
                cache[idx] = 1;
            } else if (i == 1) {
                cache[idx] = -1;
            } else if (i == 2) {
                cache[idx] = 0;
            } else {
                /* tmp = binomial(i-1,2) * a(i-3) */
                seq_t bin_mid = ((seq_t)(i-1) * (seq_t)(i-2)) / 2;
                seq_t tmp = bin_mid * cache[idx-3];

                /* sum = Sum_{j=0}^{i-1} binomial(i-1,j) * a(i-j-1) */
                seq_t sum = 0;
                seq_t bin   = 1; /* binomial(i-1,0) */
                for (idx_t j = 0; j <= i-1; ++j) {
                    sum += bin * cache[idx-j-1];
                    if (j + 1 < i) {
                        bin *= (seq_t)(i-1-j);
                        bin /= (seq_t)(j+1);
                    }
                }

                cache[idx] = tmp - sum;
            }
        }
        cache_size = new_size;
    }
    return cache[(size_t)n];
}
