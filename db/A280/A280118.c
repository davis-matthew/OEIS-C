/*
 * A280118  Number of n X 2 0..2 arrays with no element unequal to a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors,
 *          with the exception of exactly one element, and with new values
 *          introduced in order 0 sequentially upwards.
 * https://oeis.org/A280118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(n) = 2*a(n-1) + 7*a(n-2) - 24*a(n-4) - 32*a(n-5) - 16*a(n-6)
   for n > 8.  Initial terms are
   a(1)=0, a(2)=4, a(3)=8, a(4)=28, a(5)=80, a(6)=252,
   a(7)=776, a(8)=2356. */
seq_t a(idx_t n) {
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated capacity in elements   */
    static size_t cache_len = 0;          /* highest index computed (>=1)      */

    /* Ensure cache has the base values up to n=8 */
    if (cache_len < 8) {
        /* Allocate enough space for indices 0..8 */
        size_t needed = 9;                 /* 0 is unused, we keep it for convenience */
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Initialise the first 8 terms (index 0 is a dummy) */
        cache[0] = 0;                      /* unused */
        cache[1] = 0;
        cache[2] = 4;
        cache[3] = 8;
        cache[4] = 28;
        cache[5] = 80;
        cache[6] = 252;
        cache[7] = 776;
        cache[8] = 2356;
        cache_len = 8;
    }

    /* Grow cache if the requested index is outside current capacity */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 8) continue;      /* already initialised */
        seq_t term1 = 2 * cache[i - 1] + 7 * cache[i - 2];
        seq_t term2 = 24 * cache[i - 4] + 32 * cache[i - 5] + 16 * cache[i - 6];
        cache[i] = term1 - term2;
    }
    cache_len = n;

    return cache[n];
}
