/*
 * A309523  Start with a(1) = 1 and apply certain patterns of operations on
 *          a(n-1) to obtain a(n) as described in comments.
 * https://oeis.org/A309523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1..11) = 1,7,8,2,16,4,5,17,10,34,35 a(n) = a(n-1) - a(n-2) +
 *            a(n-3) + 4*a(n-8) - 4*a(n-9) + 4*a(n-10) - 4*a(n-11) for n>11
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 116, u64 n <= 244,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial 11 terms (index 1..11) */
    static const seq_t init[11] = {
        (seq_t)1, (seq_t)7, (seq_t)8, (seq_t)2,
        (seq_t)16, (seq_t)4, (seq_t)5, (seq_t)17,
        (seq_t)10, (seq_t)34, (seq_t)35
    };

    /* Latch a static cache that grows as needed */
    static seq_t *cache = NULL;       /* array indexed from 1 */
    static idx_t  cached_max = 0;     /* largest index stored */
    static size_t capacity  = 0;      /* allocated size (1‑based) */

    /* Handle small n with the hard‑coded values */
    if (n <= 11)
        return init[n-1];

    /* Initialise cache on first call */
    if (cache == NULL) {
        capacity = 12; /* indices 1..11 + 1 unused 0 slot */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) abort();
        for (idx_t i = 1; i <= 11; ++i)
            cache[i] = init[i-1];
        cached_max = 11;
    }

    /* Grow the cache if needed */
    if (n > capacity - 1) {
        size_t new_cap = (size_t)n + 1; /* 1‑based */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t val = cache[i-1] - cache[i-2] + cache[i-3]
                  + (seq_t)4 * cache[i-8]
                  - (seq_t)4 * cache[i-9]
                  + (seq_t)4 * cache[i-10]
                  - (seq_t)4 * cache[i-11];
        cache[i] = val;
    }
    cached_max = n;
    return cache[n];
}
