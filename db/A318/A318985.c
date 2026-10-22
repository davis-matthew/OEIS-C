/*
 * A318985  Theta series of quadratic form x^2 + x*y + 41*y^2.
 * https://oeis.org/A318985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A318985: number of integer solutions of x^2 + x*y + 41*y^2 = n */
#include <stdint.h>
#include <stdlib.h>

/* Integer square root of a 64‑bit unsigned integer.
   Uses binary search with 128‑bit multiplication to avoid overflow. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((__int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of entries stored (=max_n + 1) */

    /* whenever the requested index is out of current bounds, recompute up to new_max */
    if (n >= cache_size) {
        idx_t new_max   = n;
        size_t new_size = (size_t)new_max + 1;
        seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!new_cache)   /* allocation failure – unlikely in tests */
            return 0;

        /* bounds for y and x */
        int64_t y_max = (int64_t)isqrt_u64(new_max / 41u);
        int64_t x_max = (int64_t)isqrt_u64(new_max);

        /* iterate over all (x, y) pairs that could produce n ≤ new_max */
        for (int64_t y = -y_max; y <= y_max; ++y) {
            int64_t y_sq = y * y;
            int64_t y_term = 41 * y_sq;
            for (int64_t x = -x_max; x <= x_max; ++x) {
                int64_t val = x * x + x * y + y_term;
                if (val >= 0 && (idx_t)val <= new_max) {
                    new_cache[(idx_t)val]++;    /* count this solution */
                }
            }
        }

        /* replace old cache with the new one */
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[(idx_t)n];
}
