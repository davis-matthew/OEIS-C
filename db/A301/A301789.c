/*
 * A301789  Number of n X 7 0..1 arrays with every element equal to 0 or 1
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 35, u64 n <= 74, u128 n <= 153,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-4) - a(n-6),  n >= 7
   Initial values (n = 1..6):
   a(1) = 21, a(2) = 35, a(3) = 59, a(4) = 100,
   a(5) = 171, a(6) = 294
   The function memoizes results so calls in any order are O(n) total. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    /* base values stored as 64‑bit constants to avoid overflow in literal */
    static const uint64_t base[] = {0ULL, 21ULL, 35ULL, 59ULL,
                                    100ULL, 171ULL, 294ULL};

    /* Memoisation cache for a(1)..a(cache_len) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static int initialized = 0;

    /* Initialise cache with the six known values once */
    if (!initialized) {
        cache = (seq_t *)realloc(NULL, (6 + 1) * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 1; i <= 6; ++i)
            cache[i] = (seq_t)base[i];
        cache_len = 6;
        initialized = 1;
    }

    /* Grow the cache if the requested index is beyond what we have */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* Compute new values using the recurrence.  All needed
           previous indices are guaranteed to exist. */
        for (size_t i = cache_len + 1; i <= new_len; ++i) {
            cache[i] = (cache[i - 1] * (seq_t)2) - cache[i - 4] - cache[i - 6];
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
