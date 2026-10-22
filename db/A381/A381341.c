/*
 * A381341  Expansion of e.g.f. exp( x * cosh(sqrt(2)*x) ).
 * https://oeis.org/A381341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * a(n) = Sum_{k=0..floor((n-1)/2)} 2^k * (2*k+1) *
 *            binomial(n-1,2*k) * a(n-2*k-1) * The implementation memoises all
 *            terms in a static cache so that calls in any order are * O(n^2)
 *            overall and each term is computed exactly (for
 *            arbitrary-precision builds) * using only integer arithmetic
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* capacity of cache array */
    static size_t len = 0;   /* next free index in cache */

    /* Ensure cache initialised for the first call */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1; /* a(0) = 1 */
        len  = 1;
    }

    /* If n is already computed, just return it */
    if ((idx_t)len > n) {
        return cache[n];
    }

    /* Grow the cache until it can hold index n */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap;
        while ((idx_t)new_cap <= n) {
            new_cap *= 2;
        }
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t sum = 0;
        idx_t kmax = (i - 1) >> 1;           /* floor((i-1)/2) */
        seq_t pow2k = 1;                     /* 2^k */
        seq_t binom_even = 1;                  /* C(i-1, 0) */

        for (idx_t k = 0; k <= kmax; ++k) {
            idx_t j = 2 * k;                 /* 2k */
            idx_t prev = i - j - 1;          /* index of a(i-2k-1) */

            /* term = 2^k * (2k+1) * C(i-1,2k) * a(i-2k-1) */
            seq_t term = binom_even;
            term = term * ((seq_t)(2 * k + 1));
            term = term * pow2k;
            term = term * cache[prev];
            sum = sum + term;

            /* Update binomial for next k (if any) */
            if (k < kmax) {
                seq_t factor1 = (seq_t)(i - 1 - j);          /* (i-1-j)   */
                seq_t factor2 = (seq_t)(i - 1 - j - 1);      /* (i-1-j-1) */
                seq_t denom   = (seq_t)(j + 2) * (seq_t)(j + 1); /* (j+2)*(j+1) */
                binom_even = (binom_even * factor1 * factor2) / denom;
            }

            /* Next power of two */
            pow2k = pow2k * (seq_t)2;
        }

        cache[i] = sum;
    }

    len = n + 1;
    return cache[n];
}
