/*
 * A310503  Coordination sequence Gal.4.73.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A310503: a(0) = 1, a(1) = 4, a(2) = 10, a(3) = 16, a(4) = 20,
   a(5) = 26, a(6) = 32, a(7) = 36, a(8) = 40, a(n) = a(n-1) + a(n-7) - a(n-8)
   for n > 8.  This implementation uses memoisation and a dynamic cache
   that grows multiplicatively.  It works in C11 and C++17.
   The cache is updated lazily so that calls in any order are handled. */

#include <stdlib.h>

/* ---------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Base values up to n = 8. */
    static const seq_t base[9] = {1, 4, 10, 16, 20, 26, 32, 36, 40};

    /* Handle small indices directly. */
    if (n < 9)
        return base[(size_t)n];

    /* Static state for memoisation. */
    static seq_t *cache = NULL;
    static size_t cached_len = 8;   /* largest index computed so far */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* Allocate or grow the cache so that it can hold index n. */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;                /* allocation failure */
        /* Initialise first nine entries. */
        for (size_t i = 0; i < 9; ++i)
            cache[i] = base[i];
    } else if (cache_cap <= (size_t)n) {
        /* Double the capacity until it is large enough. */
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing values up to n. */
    if ((size_t)n > cached_len) {
        for (size_t i = cached_len + 1; i <= (size_t)n; ++i) {
            /* recurrence a(i) = a(i-1) + a(i-7) - a(i-8) */
            cache[i] = cache[i - 1] + cache[i - 7] - cache[i - 8];
        }
        cached_len = (size_t)n;
    }

    return cache[(size_t)n];
}
