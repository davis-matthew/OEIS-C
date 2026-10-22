/*
 * A344399  a(n) = 4^n*binomial(n - 1/2, -1/2)*(n^2 + 1).
 * https://oeis.org/A344399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n * binomial(n - 1/2, -1/2) * (n^2 + 1) (OEIS A344399)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1,
               a(n) = a(n-1) * (-2 + 4*n - 2*n^2 + 4*n^3) /
                                 (2*n - 2*n^2 + n^3)   for n > 0
   The recurrence can be rewritten as
               a(n) = a(n-1)/den * num
   where
               num = 4*n^3 - 2*n^2 + 4*n - 2
               den = n^3 - 2*n^2 + 2*n
   The division is exact for all n ≥ 1. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache for computed terms */
static size_t cache_size = 0;        /* number of terms currently cached */

/* Ensure the cache contains terms up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)               /* already computed */
        return;

    /* Grow the cache array if necessary */
    size_t new_cap = cache_size ? cache_size : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL)            /* allocation failed */
        abort();
    cache = new_cache;

    /* Initialise new elements to zero (sequential overwrite will follow) */
    for (size_t i = cache_size; i < new_cap; ++i)
        cache[i] = 0;

    /* Compute missing terms */
    if (cache_size == 0) {            /* cache is empty: initialise a(0) */
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Start computation from the next missing index */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t N = (seq_t)i;

        /* den = n^3 - 2*n^2 + 2*n  */
        seq_t den = N * N * N - (seq_t)2 * N * N + (seq_t)2 * N;

        /* num = 4*n^3 - 2*n^2 + 4*n - 2  */
        seq_t num = (seq_t)4 * N * N * N - (seq_t)2 * N * N
                    + (seq_t)4 * N - (seq_t)2;

        seq_t prev  = cache[i - 1];
        seq_t step  = prev / den;          /* exact division */
        cache[i] = step * num;             /* final value */
    }
    cache_size = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
