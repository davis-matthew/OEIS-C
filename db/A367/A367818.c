/*
 * A367818  Expansion of e.g.f. exp(1 - 3*x - exp(x)).
 * https://oeis.org/A367818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = -3*a(n-1) - Sum_{k=1..n}
 *            binomial(n-1,k-1)*a(n-k) for n >= 1
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 18, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static idx_t cache_len = 0;
    static seq_t *cache = NULL;

    /* initialize cache with a(0) */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
    }

    /* grow cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        /* compute new terms */
        for (idx_t k = cache_len; k <= n; ++k) {
            if (k == 0) {           /* already set */
                cache[0] = (seq_t)1;
                continue;
            }
            idx_t m = k - 1;         /* we need binom(m, j) for j=0..m */
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;   /* binom(m,0) */

            for (idx_t j = 0; j <= m; ++j) {
                /* term binom(m,j) * a(m-j) */
                sum += binom * cache[m - j];

                /* construct next binomial if needed */
                if (j < m) {
                    binom = binom * (seq_t)(m - j) / (seq_t)(j + 1);
                }
            }

            seq_t temp = (seq_t)3 * cache[k - 1];
            temp = -temp;             /* -3*a(k-1) */
            cache[k] = temp - sum;    /* -3*a(k-1) - sum */
        }

        cache_len = new_len;
    }

    return cache[(size_t)n];
}
