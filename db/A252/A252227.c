/*
 * A252227  Number of (n+2) X (7+2) 0..2 arrays with every 3 X 3 subblock row
 *          and column sum 2 3 or 4 and every diagonal and antidiagonal sum
 *          not 2 3 or 4.
 * https://oeis.org/A252227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + 3*a(n-3) + 3*a(n-4) - a(n-5) - a(n-6)
 *            for n > 8, with initial values a(1) = 9922, a(2) = 219824, a(3)
 *            = 718514, a(4) = 1619714, a(5) = 4297634, a(6) = 13842872, a(7)
 *            = 44099648, a(8) = 135348914
 * Range:     u8 none, u16 n <= 1, u32 n <= 11, u64 n <= 30, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage required for the compile environment */
seq_t a(idx_t n)
{
    /* Initial terms for 1 .. 8 (index 0 unused) */
    static const seq_t init[9] = {
        0ULL,          /* unused */
        9922ULL,
        219824ULL,
        718514ULL,
        1619714ULL,
        4297634ULL,
        13842872ULL,
        44099648ULL,
        135348914ULL
    };

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_up_to = 0;   /* largest index for which cache[i] is valid */

    /* Handle indices <=8 directly */
    if (n <= 8)
        return init[(idx_t)n];

    /* Grow the cache if necessary */
    if (n > cached_up_to) {
        /* Allocate or reallocate */
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_size + 1));
        if (tmp == NULL) {
            /* In a real application error handling would go here */
            return 0;
        }
        cache = tmp;

        /* Bring initial values into the cache if not already present */
        if (cached_up_to < 8) {
            for (idx_t i = cached_up_to + 1; i <= 8; ++i)
                cache[i] = init[i];
        }

        /* Ensure the new entries are initially zero (indicate uncomputed) */
        for (idx_t i = cached_up_to + 1; i <= new_size; ++i)
            if (i > 8)  /* indices 1..8 already set */
                cache[i] = 0;

        cached_up_to = new_size;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = 9; i <= n; ++i) {
        if (cache[i] != 0)   /* already computed */
            continue;
        cache[i] = 3*cache[i-1] - cache[i-2] + 3*cache[i-3]
                 + 3*cache[i-4] - cache[i-5] - cache[i-6];
    }

    return cache[n];
}
