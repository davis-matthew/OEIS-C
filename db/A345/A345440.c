/*
 * A345440  Represent the ring R = {x+y*sqrt(-2): x, y rational integers} by
 *          the cells centered at the points (x,y) of a square grid, as in
 *          A345437; number the cells of the grid along a counterclockwise
 *          square spiral, with the cells at (0,0) and (1,0) numbered 0, 1;
 *          then a(n) is the norm x^2+2*y^2 of the ele...
 * https://oeis.org/A345440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A174344(n+1)^2 + 2*A274923(n+1)^2
 * Range:     u8 n <= 360, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the norm x^2 + 2*y^2 of the element at the n-th cell
   of the square spiral on the lattice {x + y*sqrt(-2)}. */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for 0 <= n < 2^64. */
static idx_t isqrt_u64(idx_t n)
{
    if (n == 0 || n == 1)
        return n;

    idx_t low = 0;
    idx_t high = 0xFFFFFFFFULL;          /* sqrt(2^64-1) < 2^32 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            low = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Convert to 1‑based index m = n + 1. */
    idx_t m = n + 1;

    /* Determine the ring r (r = 0 for the origin). */
    idx_t r;
    if (m == 1) {
        r = 0;
    } else {
        /* Rough estimate using sqrt. */
        idx_t s = isqrt_u64(m);
        r = (s - 1) / 2;

        /* Verify that the cumulative count for this r reaches m. */
        __uint128_t cum = (__uint128_t)4 * r * (r + 1) + 1;
        if (cum < m)
            ++r;
    }

    /* Origin case. */
    if (r == 0)
        return (seq_t)0;

    /* Index of the first cell of ring r. */
    idx_t startIdx = 2 + 4 * r * (r - 1);

    /* Offset within the ring. */
    idx_t t = m - startIdx;   /* 0 <= t < 8*r */

    /* Compute coordinates as 64‑bit signed integers. */
    long long x, y;
    if (t == 0) {                    /* first cell of the ring */
        x = (long long)r;
        y = -(long long)(r - 1);
    } else if (t <= 2 * r - 1) {      /* moving up along the right side */
        x = (long long)r;
        y = -(long long)(r - 1) + (long long)t;
    } else if (t <= 4 * r - 1) {      /* moving left along the top side */
        long long offset = (long long)t - 2LL * (long long)r;
        x = (long long)(r - 1 - offset);
        y = (long long)r;
    } else if (t <= 6 * r - 1) {      /* moving down along the left side */
        long long offset = (long long)t - 4LL * (long long)r;
        x = -(long long)r;
        y = (long long)(r - 1 - offset);
    } else {                          /* moving right along the bottom side */
        long long offset = (long long)t - 6LL * (long long)r;
        x = -(long long)r + 1 + offset;
        y = -(long long)r;
    }

    /* Compute the norm x^2 + 2*y^2. */
    seq_t ax = (x >= 0) ? (seq_t)x : (seq_t)(-x);
    seq_t ay = (y >= 0) ? (seq_t)y : (seq_t)(-y);
    seq_t nx = ax * ax;
    seq_t ny = ay * ay;
    return nx + (seq_t)2 * ny;
}
