/*
 * A340888  a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(n,k)^2 * 4^(n-k-1) *
 *          a(k).
 * https://oeis.org/A340888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1;
 * a(n) = Sum_{k=0..n-1} binomial(n,k)^2 * 4^(n-k-1) * a(k)
 * (Recursive definition)
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

#include "../utils/pow4.h"   /* provides seq_t pow4(idx_t exp) */

/* Static cache for memoisation */
static seq_t *cache      = NULL;   /* array holding a(0)..a(max) */
static size_t cache_size = 0;      /* number of elements currently allocated */
static idx_t   cache_max  = 0;      /* highest index that has been computed */

/* Ensure that the cache can hold values up to index n */
static void extend_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_size)
        return;

    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)        /* allocation failed – let the caller handle it */
        return;

    /* initialise newly allocated slots to 0, meaning “not yet computed” */
    for (size_t i = cache_size; i < new_size; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;
    cache_size = new_size;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* initialise static variables on first call */
    if (cache == NULL) {
        cache = NULL;
        cache_size = 0;
        cache_max = 0;
    }

    /* ensure space for requested index */
    extend_cache(n);

    /* base case */
    if (n == 0)
        return (seq_t)1;

    /* ensure a(0) is stored as 1 */
    if (cache[0] == (seq_t)0)
        cache[0] = (seq_t)1;
    cache_max = 0;   /* a(0) has been stored */

    /* compute any missing terms up to n, in increasing order */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;

        /* initialise binomial(m,0) = 1 and pow4(m-1) = 4^(m-1) */
        seq_t binom = (seq_t)1;
        seq_t pow4_val = pow4(m - 1);   /* 4^(m-1) */

        for (idx_t k = 0; k < m; ++k) {
            /* term = binom^2 * pow4_val * a(k) */
            seq_t term = binom * binom;      /* binom^2 */
            term = term * pow4_val;          /* *4^(m-k-1) */
            term = term * cache[k];          /* *a(k) */

            sum = sum + term;                /* accumulate */

            /* update binomial and pow4 for next k */
            if (k + 1 < m) {
                binom = binom * (seq_t)(m - k);        /* * (m-k) */
                binom = binom / (seq_t)(k + 1);        /* / (k+1) */
                pow4_val = pow4_val / (seq_t)4;         /* divide by 4 for next exponent */
            }
        }

        cache[m] = sum;
        cache_max = m;
    }

    return cache[n];
}
