/*
 * A318180  Expansion of e.g.f. exp((1 - exp(-5*x))/5).
 * https://oeis.org/A318180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-5)^(n-k) * Stirling2(n,k) Recurrence (used
 *            for efficient computation): a(0) = 1; for n >= 1: a(n) =
 *            Σ_{j=0}^{n-1} (-5)^j * binomial(n-1,j) * a(n-1-j) The function
 *            caches results to allow arbitrary call order
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 12, i64 n <= 18, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t computed_until = 0; /* indices [0 .. computed_until-1] are initialized */
    static size_t cache_len = 0;     /* allocated size of cache */

    /* ensure cache can hold indices up to n */
    if (n + 1 > cache_len) {
        size_t new_len = n + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_len];
#else
        new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
#endif
        /* copy previous values */
        if (cache) {
            for (idx_t i = 0; i < cache_len; ++i) {
                new_cache[i] = cache[i];
            }
#ifdef __cplusplus
            delete [] cache;
#else
            free(cache);
#endif
        }
        cache     = new_cache;
        cache_len = new_len;
    }

    /* initialize a(0) once */
    if (computed_until == 0) {
        cache[0] = (seq_t)1;
        computed_until = 1;
    }

    /* if the requested value is already known, return it */
    if (n < computed_until) {
        return cache[n];
    }

    /* compute missing values up to n */
    for (idx_t m = computed_until; m <= n; ++m) {
        if (m == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;   /* binomial(n-1,0) */
        seq_t power = (seq_t)1;   /* (-5)^0 */

        for (idx_t j = 0; j <= m - 1; ++j) {
            /* term: (-5)^j * binomial(m-1,j) * a(m-1-j) */
            sum += power * binom * cache[m - 1 - j];

            /* update binomial for next j if needed */
            if (j < m - 1) {
                binom = binom * (seq_t)(m - 1 - j);
                binom = binom / (seq_t)(j + 1);
            }

            /* update power (-5)^(j+1) */
            power = power * (seq_t)(-5);
        }

        cache[m] = sum;
    }

    computed_until = n + 1;
    return cache[n];
}
