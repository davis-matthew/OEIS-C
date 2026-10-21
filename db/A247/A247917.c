/*
 * A247917  Expansion of 1 / (1 + x - x^3) in powers of x.
 * https://oeis.org/A247917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 35, i16 n <= 75, i32 n <= 155, i64 n <= 312,
 *            i128 n <= 628, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=1, a(1)=-1, a(2)=1
   For n≥3:   a(n) = a(n-3) - a(n-1)       (from 0 = a(n) – a(n+2) – a(n+3)) */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial indices directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)1;

    /* Static cache for memoisation. */
    static seq_t *cache      = NULL;  /* array of computed values */
    static size_t cache_cap = 0;      /* allocated capacity */
    static idx_t  cache_len = 0;      /* number of values stored (max index+1) */

    /* Initialise cache on first use.  We store the first three values. */
    if (cache == NULL) {
        /* enough room for at least the requested index plus some slack */
        cache_cap = (n > 10 ? n + 10 : 16);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();            /* allocation failed – terminate */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)1;
        cache_len = 3;                   /* indices 0..2 are defined */
    }

    /* Grow the cache if we need to store more values. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();        /* allocation failed – terminate */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute values up to the requested index using the recurrence. */
    if (n >= cache_len) {
        idx_t i;
        for (i = cache_len; i <= n; ++i) {
            /* a(i) = a(i-3) - a(i-1) */
            cache[i] = cache[i - 3] - cache[i - 1];
        }
        cache_len = n + 1;       /* now indices 0..n are valid */
    }

    return cache[n];
}
