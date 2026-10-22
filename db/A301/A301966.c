/*
 * A301966  Number of 4Xn 0..1 arrays with every element equal to 0, 1 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-3) - a(n-4) - 2*a(n-6) + a(n-7) for n>9
 *            a(1) = 8, a(2) = 34, a(3) = 40, a(4) = 64, a(5) = 111, a(6) =
 *            219, a(7) = 443, a(8) = 904, a(9) = 1860
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 59, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* cache[0] unused, cache[1..k] holds a(1..k) */
    static idx_t cache_len = 0;      /* number of entries stored (highest index) */

    /* Special case: n == 0 is outside the OEIS offset; return 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Initialise cache the first time. */
    if (cache_len == 0) {
        /* Allocate space for indices 0..9 (index 0 unused). */
        cache = (seq_t *)malloc(10 * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* OOM: return zero. */

        /* Initial values from the OEIS entry.  */
        static const seq_t init[10] = {0, 8, 34, 40, 64, 111, 219, 443, 904, 1860};
        for (idx_t i = 1; i <= 9; ++i) {
            cache[i] = init[i];
        }
        cache_len = 9;
    }

    /* If the requested n is already in the cache, return it. */
    if (n <= cache_len) {
        return cache[n];
    }

    /* Resize cache to hold indices up to n. */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) {
        return (seq_t)0;   /* OOM: return zero. */
    }
    cache = new_cache;

    /* Compute missing values up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 9) {
            /* These initial values are already known; skip recurrence. */
            /* They are already set from the first allocation. */
        } else {
            /* Apply the recurrence:  a(i) = 2*a(i-1) + a(i-3)
             *                      - a(i-4) - 2*a(i-6) + a(i-7) */
            cache[i] = 2 * cache[i - 1]
                       + cache[i - 3]
                       - cache[i - 4]
                       - 2 * cache[i - 6]
                       + cache[i - 7];
        }
    }

    cache_len = n;
    return cache[n];
}
