/*
 * A320136  Number of integer solutions to a^2 + b^2 + 2*c^2 + 14*d^2 = n.
 * https://oeis.org/A320136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of integer solutions to a² + b² + 2c² + 14d² = n (OEIS
 *            A320136)
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* idx_t and seq_t are defined by the build system. */

/* Integer square‑root:  floor(sqrt(n)) for non‑negative n. */
static inline idx_t isqrt_idx_t(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0) {
            ans = 0;
            low = 1;
            continue;
        }
        if (mid > n / mid)
            high = mid - 1;
        else {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

/* Cache for the count of representations of i as a² + b². */
static seq_t *r2_cache = NULL;   /* r2_cache[i] holds the count for i */
static idx_t r2_cap = 0;         /* size of cache (indices 0 .. r2_cap-1) */

/* Ensure r2_cache holds values up to (n-1).  Recompute everything. */
static void ensure_r2(idx_t n)
{
    if (n <= r2_cap)
        return;
    idx_t new_cap = n;
    seq_t *new_cache = (seq_t *)realloc(r2_cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);   /* allocation failed */

    /* Zero‑fill the entire new cache. */
    for (idx_t i = 0; i < new_cap; ++i)
        new_cache[i] = (seq_t)0;

    /* Compute all r2 values up to new_cap-1. */
    idx_t limit = isqrt_idx_t(new_cap - 1);
    for (int a = -(int)limit; a <= (int)limit; ++a) {
        idx_t a2 = (idx_t)(a * a);
        for (int b = -(int)limit; b <= (int)limit; ++b) {
            idx_t b2 = (idx_t)(b * b);
            idx_t sum = a2 + b2;
            if (sum < new_cap)
                new_cache[(size_t)sum] += (seq_t)1;
        }
    }

    r2_cache = new_cache;
    r2_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t count = (seq_t)0;

    /* d loop */
    idx_t maxD = isqrt_idx_t(n / 14);
    for (int d = -(int)maxD; d <= (int)maxD; ++d) {
        idx_t d2 = (idx_t)(d * d);
        idx_t rem1 = n - 14 * d2;
        if (rem1 > n)  /* safety guard */
            continue;
        /* c loop */
        idx_t maxC = isqrt_idx_t(rem1 / 2);
        for (int c = -(int)maxC; c <= (int)maxC; ++c) {
            idx_t c2 = (idx_t)(c * c);
            idx_t rem2 = rem1 - 2 * c2;
            if (rem2 > rem1)  /* safety guard */
                continue;
            ensure_r2(rem2 + 1);
            count += r2_cache[(size_t)rem2];
        }
    }

    return count;
}
