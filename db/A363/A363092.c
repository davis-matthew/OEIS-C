/*
 * A363092  a(n) = 4*a(n-1) - 8*a(n-2) with a(0) = a(1) = 1.
 * https://oeis.org/A363092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-2), a(0) = a(1) = 1
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 21, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc, abort */
#include <stddef.h>   /* size_t */

/* Static cache of previously computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Ensure the cache contains terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_len)
        return;   /* Already computed */

    /* Resize the cache; preserve existing contents */
    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (new_cache == NULL) {
        abort();  /* Out of memory */
    }
    cache = new_cache;

    /* Initialize the base terms if needed */
    if (needed > 0)       cache[0] = (seq_t)1;
    if (needed > 1)       cache[1] = (seq_t)1;

    /* Compute any newly required terms using the recurrence */
    size_t start = (cache_len > 1 ? cache_len : 2);
    for (size_t i = start; i < needed; ++i) {
        cache[i] = cache[i-1] * 4 - cache[i-2] * 8;
    }

    cache_len = needed;
}

/* Public entry point */
seq_t a(idx_t n)
{
    if ((size_t)n >= cache_len)
        ensure_cache(n);
    return cache[(size_t)n];
}
