/*
 * A267213  Number of OFF (white) cells in the n-th iteration of the "Rule
 *          109" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 306, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A267213: Number of OFF cells in Rule 109 starting from a single ON cell.
   Recurrence:  a(0)=0, a(1)=2, a(2)=0, a(3)=5, a(4)=2, a(5)=4, a(6)=3, a(7)=10,
                a(n) = a(n-3) + a(n-4) - a(n-7)   for n > 7          */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t  cap    = 0;          /* number of elements stored  (size of cache) */

    /* initial values up to n = 7 */
    if (cap == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 8);
        if (!cache) { /* allocation failure – return 0 as safe fallback */
            return (seq_t)0;
        }
        cache[0] = 0;  /* a(0) */
        cache[1] = 2;  /* a(1) */
        cache[2] = 0;  /* a(2) */
        cache[3] = 5;  /* a(3) */
        cache[4] = 2;  /* a(4) */
        cache[5] = 4;  /* a(5) */
        cache[6] = 3;  /* a(6) */
        cache[7] = 10; /* a(7) */
        cap = 8;       /* we have indices 0..7 stored */
    }

    /* if the requested n is already cached, return it */
    if ((idx_t)cap > n) {
        return cache[(size_t)n];
    }

    /* expand cache to cover n */
    size_t old_cap = cap;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failure – keep previous cache and return 0 */
        return (seq_t)0;
    }
    cache = tmp;
    cap = new_cap;

    /* compute missing terms using recurrence */
    for (idx_t i = (idx_t)old_cap; i <= n; ++i) {
        /* all indices i-3, i-4, i-7 are valid because i >= old_cap >= 8 */
        cache[(size_t)i] = cache[(size_t)(i - 3)] + cache[(size_t)(i - 4)] - cache[(size_t)(i - 7)];
    }

    return cache[(size_t)n];
}
