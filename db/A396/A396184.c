/*
 * A396184  Irregular triangle read by rows: row 0 is 0, row n for n >= 1 is
 *          6*n^2-3*n+k for k = 1..6*n.
 * https://oeis.org/A396184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0) = 0, for n >= 1 find the unique r >= 1 with * 1 + 3*r*(r+1)
 *            > n >= 1 + 3*(r-1)*r and * a(n) = 6*r*r - 3*r + (n - (1 +
 *            3*(r-1)*r)) + 1. * This implements the irregular triangle
 *            defined in A396184
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* helper to compute T(r) = 1 + 3*r*(r+1) */
static inline seq_t triangular(idx_t r)
{
    seq_t rr = r;
    return (seq_t)1 + (seq_t)3 * rr * (rr + (seq_t)1);
}

seq_t a(idx_t n)
{
    /* Handle the first row separately */
    if (n == 0)
        return (seq_t)0;

    /* Find smallest r >= 1 with T(r) > n  (binary search) */
    idx_t low  = 1;
    idx_t high = 1;

    /* Grow high until T(high) > n */
    while (triangular(high) <= n)
    {
        high <<= 1;
        /* Avoid infinite loop if high becomes too large */
        if (high == 0) /* overflow of idx_t, sufficient for all n */
            break;
    }

    /* Binary search in [low, high] */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) > n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t r = low; /* r >= 1 and satisfies T(r-1) <= n < T(r) */

    /* Compute previous count: T(r-1) = 1 + 3*(r-1)*r */
    seq_t rr    = r;
    seq_t prev  = (seq_t)1 + (seq_t)3 * (rr - (seq_t)1) * rr;
    seq_t k     = (seq_t)n - prev;          /* 0 <= k <= 6*r-1 */
    seq_t base  = (seq_t)6 * rr * rr - (seq_t)3 * rr; /* 6*r*r - 3*r */
    return base + k + (seq_t)1;         /* value of a(n) */
}
