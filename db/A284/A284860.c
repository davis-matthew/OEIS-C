/*
 * A284860  Alternating row sums of the Sheffer triangle (exp(x), exp(3*x) -
 *          1) given in A282629.
 * https://oeis.org/A284860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula (recurrence): 
 *   a(0) = 1
 *   a(n) = a(n-1) - Σ_{k=1..n} binomial(n-1, k-1) * 3^k * a(n-k),  n ≥ 1
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values.
       cache[0] = a(0) = 1. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity in elements */
    static size_t cache_len = 0;   /* number of values currently stored */

    /* Ensure space for n+1 entries. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort immediately. */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first element if cache empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* If requested value already computed, return it. */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Compute successive terms up to n (inclusive). */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;        /* binomial(i-1, k-1) when k=1 */
        seq_t pow3 = 3;         /* 3^k for k=1 */

        for (idx_t k = 1; k <= i; ++k) {
            /* term = binom * 3^k * a(i-k) */
            seq_t term = binom * pow3;
            term *= cache[i - k];
            sum += term;

            /* Update binomial coefficient for next k (if any). */
            if (k < i) {
                seq_t numer = (seq_t)(i - 1 - (k - 1));   /* (i-1)-(k-1) */
                seq_t denom = (seq_t)k;                  /* k */
                binom = binom * numer / denom;           /* exact division */
            }

            /* Update 3^k for next iteration. */
            pow3 *= 3;
        }

        cache[i] = cache[i - 1] - sum;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
