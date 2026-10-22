/*
 * A396475  Expansion of g.f. (1 - 11*x^2 - 20*x^4)/(1 - 2*x - 11*x^2 - 8*x^3
 *          - 20*x^4 + 10*x^5).
 * https://oeis.org/A396475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 11*a(n-2) + 8*a(n-3) + 20*a(n-4) - 10*a(n-5)
 *            with a(0)=1, a(1)=2, a(2)=4, a(3)=38, a(4)=136
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: idx_t is an unsigned 64‑bit type, seq_t is an unsigned integer
   type of 8,16,32,64 or 128 bits or an arbitrary‑precision integer. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Expand cache to accommodate index n (inclusive) */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return zero (should not happen in tests) */
            return (seq_t)0;
        }
        cache = tmp;

        /* Initialise initial terms that may not yet be set */
        for (size_t i = cache_len; i <= 4 && i < new_len; ++i) {
            switch (i) {
                case 0: cache[0] = 1; break;
                case 1: cache[1] = 2; break;
                case 2: cache[2] = 4; break;
                case 3: cache[3] = 38; break;
                case 4: cache[4] = 136; break;
            }
        }

        /* Compute terms from the first missing index up to n */
        size_t start = (cache_len > 5) ? cache_len : 5;
        for (size_t i = start; i < new_len; ++i) {
            cache[i] =
                cache[i - 1] * 2
              + cache[i - 2] * 11
              + cache[i - 3] * 8
              + cache[i - 4] * 20
              - cache[i - 5] * 10;
        }

        cache_len = new_len;
    }

    return cache[(size_t)n];
}
