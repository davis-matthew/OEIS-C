/*
 * A354391  Expansion of e.g.f. 1/(1 + (exp(x) - 1)^2 / 2).
 * https://oeis.org/A354391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = - Σ_{k=1..n} binomial(n,k) * (2^{k-1} - 1) *
 *            a(n-k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache, indices 0 .. cache_cap-1 are already computed.  */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* Make sure that all values up to n are available. */
    if (n >= cache_cap)
    {
        /* Grow the cache to hold n+1 elements. */
        if (cache == NULL)
        {
            cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
            cache_cap = n + 1;
        }
        else if (cache_cap < n + 1)
        {
            cache = (seq_t *)realloc(cache,
                                      (size_t)(n + 1) * sizeof(seq_t));
            cache_cap = n + 1;
        }

        /* Initialise a(0). */
        if (cache_cap == 1)
            cache[0] = (seq_t)1;
        else
            cache[0] = (seq_t)1;   /* ensures we have a(0) defined */

        /* Compute all new values. */
        for (idx_t cur = 1; cur <= n; ++cur)
        {
            seq_t sum   = 0;
            seq_t bin   = 1;          /* binomial(cur,0) */
            seq_t power = (seq_t)1;    /* 2^(k-1), starting with 2^0 */

            for (idx_t k = 1; k <= cur; ++k)
            {
                /* Update binomial to binomial(cur,k). */
                bin = bin * ((seq_t)(cur - k + 1)) / (seq_t)k;

                /* b(k) = 2^{k-1} - 1. */
                seq_t bk = power - 1;

                /* Term of the sum: bin * (2^{k-1}-1) * a(cur-k). */
                seq_t term = bin * bk * cache[cur - k];
                sum += term;

                /* Prepare power for next k. */
                power *= (seq_t)2;
            }

            /* a(cur) = -Σ(...) */
            cache[cur] = -sum;
        }
    }

    return cache[n];
}
