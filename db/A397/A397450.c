/*
 * A397450  a(n) is the number of positive integers <= n that are the sum of 2
 *          nonzero squares.
 * https://oeis.org/A397450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 817, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function enumerates all pairs of non‑zero integers (x,y)
   with x² + y² ≤ n, records the distinct sums, and counts how
   many of them are ≤ n.  This number of distinct sums is
   precisely a(n) – the number of positive integers ≤ n that can
   be written as the sum of two non‑zero squares.  */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer floor square root; uses a helper if available, otherwise
   a simple binary search.  */
static idx_t isqrt_u64(idx_t n)
{
    /* If a more accurate helper is provided */
#if defined(_floor_isqrt_u64)
    return _floor_isqrt_u64(n);
#else
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid)   /* mid*mid > n without overflow */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
#endif
}

seq_t a(idx_t n)
{
    if (n < 1)               /* safety for indices below the offset */
        return (seq_t)0;

    /* Allocate a boolean array for numbers 0 … n.  Index 0 is unused. */
    size_t sz = (size_t)(n + 1);
    bool *seen = (bool *)calloc(sz, sizeof(bool));
    if (!seen)               /* allocation failure – return 0 */
        return (seq_t)0;

    idx_t x_limit = isqrt_u64(n);
    for (idx_t x = 1; x <= x_limit; ++x) {
        idx_t x2 = x * x;                   /* x² ≤ n */
        idx_t rem = n - x2;                 /* non‑negative */
        idx_t y_limit = isqrt_u64(rem);
        for (idx_t y = 1; y <= y_limit; ++y) {
            idx_t sum = x2 + y * y;         /* ≤ n by construction */
            seen[(size_t)sum] = true;
        }
    }

    /* Count how many flag entries are true. */
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (seen[(size_t)i])
            ++count;
    }

    free(seen);
    return count;
}
