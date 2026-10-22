/*
 * A336210  a(0) = 1; a(n) = -(1/n) * Sum_{k=0..n-1} binomial(n,k)^3 * (n-k) *
 *          a(k).
 * https://oeis.org/A336210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n) = -(1/n) * Sum_{k=0}^{n-1} binomial(n,k)^3 * (n-k)
 *            * a(k) for n>=1 The implementation memoizes computed values so
 *            that calls in any order are fast
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 12, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The types seq_t (the sequence element type) and idx_t (index type)
   are defined by the build system. */

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;          /* cache holds values 0 .. cap-1   */

    /* If already computed, return from cache */
    if (n < cap) return cache[n];

    /* Expand cache to hold values up to n */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    /* If realloc fails, cache becomes NULL and program will abort.
       The build environment guarantees sufficient memory for test cases. */

    /* Compute new values sequentially */
    for (idx_t i = cap; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = 1;
            continue;
        }

        /* Sum over k = 0 .. i-1 */
        seq_t sum = 0;
        seq_t bin = 1;                     /* binomial(i,0) */

        for (idx_t k = 0; k < i; ++k)
        {
            /* bin3 = binomial(i,k)^3  */
            seq_t bin3 = bin * bin * bin;

            /* term = bin3 * (i-k) * a(k) */
            seq_t term = bin3 * ( (seq_t)(i - k) ) * cache[k];
            sum += term;

            /* Update bin to binomial(i,k+1) */
            if (k + 1 <= i - 1)          /* avoid unnecessary work on last step */
            {
                seq_t numer = bin * ( (seq_t)(i - k) );
                seq_t denom = (seq_t)(k + 1);
                bin = numer / denom;     /* division is exact */
            }
        }

        /* a(i) = -(sum) / i  */
        cache[i] = -sum / ( (seq_t)i );
    }

    cap = n + 1;
    return cache[(size_t)n];
}
