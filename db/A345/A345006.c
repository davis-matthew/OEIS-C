/*
 * A345006  a(0) = 1; a(3*n) = a(n) + a(n-1), a(3*n+1) = a(3*n+2) = -a(n).
 * https://oeis.org/A345006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 93, i16 n <= 93, i32 n <= 93, i64 n <= 93, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1
   a(3*n)     = a(n) + a(n-1)   (for n >= 1)
   a(3*n+1)   = -a(n)
   a(3*n+2)   = -a(n)           */

/* This implementation uses lazy linear DP with a static cache.
   It works in C11 and C++17.  The cache grows geometrically
   as needed and does not recompute terms already known. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: computed values */
    static seq_t *cache   = NULL;   /* array of computed a(i) */
    static size_t cache_len = 0;    /* number of elements currently stored (next free index) */
    static size_t cache_cap = 0;    /* allocated capacity in elements */

    /* initialise a(0) the first time */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* unsafe fallback */
        cache[0] = (seq_t)1;
        cache_len = 1;                         /* indices [0 .. 0] computed */
    }

    /* If we already have the requested value, return it immediately */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow cache to fit up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* unsafe fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to and including n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        size_t k = i / 3;
        if (i % 3 == 0) {
            /* a(3*k) = a(k) + a(k-1)   (k >= 1) */
            cache[i] = cache[k] + cache[k - 1];
        } else {
            /* a(3*k+1) and a(3*k+2) = -a(k) */
            cache[i] = -cache[k];
        }
    }

    cache_len = (size_t)n + 1;        /* all indices <= n are now computed */
    return cache[(size_t)n];
}
