/*
 * A295689  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 2, a(1) = 0, a(2) =
 *          2, a(3) = 1.
 * https://oeis.org/A295689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4) a(0) = 2, a(1) = 0, a(2) = 2,
 *            a(3) = 1
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;  /* number of valid entries in cache */

    /* Initialise base values if cache not yet allocated */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;            /* Out of memory: fallback */

        cache[0] = (seq_t)2;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
    }

    /* If the requested term is already computed, return it */
    if (n < cap)
        return cache[n];

    /* Grow the cache to accommodate up to n */
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (tmp == NULL)
        return (seq_t)0;                /* Out of memory: fallback */

    cache = tmp;
    /* Compute any new terms that are needed, starting from the first
       uninitialised index (old_cap) up to n inclusive */
    for (idx_t i = cap; i <= n; ++i) {
        /* For i >= 4, the recurrence is valid */
        seq_t v = cache[i - 1] + cache[i - 3] + cache[i - 4];
        cache[i] = v;
    }

    cap = new_cap;
    return cache[n];
}
