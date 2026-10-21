/*
 * A253832  a(n) = a(n-1) * (1 + a(n-2)/a(n-4)), a(0) = a(1) = a(2) = a(3) =
 *          1.
 * https://oeis.org/A253832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * (1 + a(n-2)/a(n-4)), a(0) = a(1) = a(2) = a(3) =
 *            1
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 11, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache the sequence values.  The cache is grown on demand. */
static seq_t *cache = NULL;          /* allocated array of seq_t values   */
static idx_t  cache_size = 0;        /* current number of allocated slots */
static idx_t  last_computed = 0;     /* highest index that has been computed */

/* Compute a(n).  The function is re-entrant and can be called with
   indices in any order.  All previously computed values are retained
   in the static cache. */
seq_t a(idx_t n)
{
    /* Initialise the cache with the first four terms (all 1). */
    if (cache_size == 0) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();   /* sanity check */
        for (idx_t i = 0; i < 4; ++i)
            cache[i] = 1;
        last_computed = 3;
    }

    /* Grow the cache so that index n is addressable. */
    if (cache_size <= n) {
        size_t new_size = cache_size ? cache_size : 4;
        while (new_size <= (size_t)n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();   /* sanity check */
        cache = tmp;
        /* initialise new slots to zero (they will be overwritten soon) */
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }

    /* Compute any terms up to n that have not been computed yet. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* All indices i >= 4 here, so a[i-4] exists. */
        seq_t term = cache[i - 1] * (1 + cache[i - 2] / cache[i - 4]);
        cache[i] = term;
    }
    last_computed = n > last_computed ? n : last_computed;

    return cache[n];
}
