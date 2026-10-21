/*
 * A250309  a(n) = a(n-1)*(1 + a(n-1)/a(n-3)), with a(0) = a(1) = a(2) = 1.
 * https://oeis.org/A250309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)*(1 + a(n-1)/a(n-3)), a(0)=a(1)=a(2)=1 */
/* Implementation with memoisation; works for any seq_t (built‑in or arbitrary‑precision) */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* current capacity of cache array */
    static idx_t cache_len = 0;    /* number of computed terms (max index + 1) */

    /* Grow the cache array if necessary */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;

        /* realloc works for both integral and class seq_t (C++ cast is allowed) */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0 || i == 1 || i == 2) {
            cache[(size_t)i] = (seq_t)1;
        } else {
            seq_t prev = cache[(size_t)(i - 1)];
            seq_t third = cache[(size_t)(i - 3)];
            seq_t q = prev / third;             /* exact division */
            cache[(size_t)i] = prev * (q + (seq_t)1);
        }
    }

    cache_len = (idx_t)(n + 1);
    return cache[(size_t)n];
}
