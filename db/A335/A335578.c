/*
 * A335578  a(0) = 1; a(n) = -Sum_{k=1..n} binomial(n,k) * k^3 * a(n-k).
 * https://oeis.org/A335578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A335578
   Recurrence: a(0) = 1;
   a(n) = -∑_{k=1..n} binomial(n,k) * k^3 * a(n-k)   (n ≥ 1)
*/

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, realloc */
#include "../utils/binomial.h"

/* Persistent cache of computed values. */
static seq_t *cache = NULL;            /* a(0)..a(max_computed) */
static size_t cache_cap = 0;           /* allocated slots */
static idx_t  max_computed = 0;         /* highest n for which cache[n] is valid */

/* Return a(n) using memoised recurrence. */
seq_t a(idx_t n)
{
    /* Grow the cache array to hold a[0..n]. */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cache_cap = n + 1;
    } else if (cache_cap <= n) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache_cap = n + 1;
    }

    /* Initialise a(0) if not already done. */
    if (max_computed == 0 && n >= 0) {
        cache[0] = (seq_t)1;
        max_computed = 0;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = binomial(i, k);          /* C(i,k)      */
            /* k^3 */
            seq_t ksq = (seq_t)k * k * k;
            term *= ksq;
            term *= cache[i - k];
            sum += term;
        }
        cache[i] = -sum;
    }
    max_computed = n;
    return cache[n];
}
