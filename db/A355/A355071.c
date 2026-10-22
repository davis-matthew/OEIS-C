/*
 * A355071  G.f.: Sum_{n>=0} a(n)*x^n/(n!*4^(n*(n-1)/2)) = log( Sum_{n>=0}
 *          x^n/(n!*4^(n*(n-1)/2)) ).
 * https://oeis.org/A355071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 7, i128 n <= 10,
 *            big any n
 * Helpers:   pow4_uint, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A355071:
   a(0) = 0;
   a(n) = 1 - Σ_{k=1}^{n-1} 4^{k*(n-k)} * C(n-1,k) * a(n-k)   (n≥1) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/pow4_uint.h"

/* Static cache for a(n). */
static seq_t *cache = NULL;       /* a[0…] */
static size_t cache_cap = 0;      /* allocated capacity */
static idx_t cache_size = 0;      /* number of elements computed */

/* Ensure that all a(0)…a(n) are computed and stored. */
static void ensure_upto(idx_t n)
{
    if (n < cache_size)
        return;                 /* already computed */

    /* Grow the cache if necessary. */
    size_t needed = (size_t)n + 1U;
    if (needed > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap : 4U;
        while (new_cap < needed)
            new_cap <<= 1U;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms. */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i == 0U)        /* a(0) = 0 */
        {
            cache[0] = 0;
            continue;
        }

        seq_t sum = 0;
        seq_t bin = 1;      /* C(i-1,0) */

        /* k runs from 1 to i-1 */
        for (idx_t k = 1U; k < i; ++k)
        {
            /* Update binomial coefficient to C(i-1,k). */
            bin = bin * (seq_t)(i - 1U - (k - 1U));
            bin = bin / (seq_t)k;

            /* Exponent for 4. */
            idx_t exp_leq = k * (i - k);

            /* 4^exp_leq */
            seq_t pow4 = pow4_uint(exp_leq);

            /* term = 4^exp_leq * C(i-1,k) * a(i-k) */
            seq_t term = pow4 * bin * cache[i - k];

            sum += term;
        }

        cache[i] = (seq_t)1 - sum;
    }

    cache_size = n + 1U;
}

/* Externally visible function. */
seq_t a(idx_t n)
{
    ensure_upto(n);
    return cache[n];
}
