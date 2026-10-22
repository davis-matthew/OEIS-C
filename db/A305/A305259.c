/*
 * A305259  x-coordinates of a point moving counterclockwise on concentric
 *          squares of grid points rotated by Pi/4 with side length m*sqrt(2),
 *          m=1,2,..., with jump to next square on the positive x-axis.
 * https://oeis.org/A305259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the x‑coordinate of a point that moves counter‑clockwise
 *            around successive squares whose vertices lie on the axes, the
 *            m‑th square having side length m*sqrt(2). The point starts at
 *            (1,0) and at the positive x‑axis it jumps to the next larger
 *            square
 * Range:     i8 n <= 84, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation works for any integer type seq_t and idx_t supplied
   by the build system.  It uses only integer arithmetic and is O(log n)
   per call.  No floating point operations or external utilities are
   required. */

#include <stdint.h>

static inline idx_t floor_sqrt_u64(idx_t x)
{
    /* floor(sqrt(x)) for 0 <= x <= 2^63-1 using binary search. */
    idx_t lo = 0, hi = ((idx_t)1 << 32); /* hi^2 > max x */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Find the minimal m such that 2*m*(m+1) >= n (count of terms up to square m). */
static inline idx_t find_m(idx_t n)
{
    idx_t low = 1;
    idx_t high = 2;
    while ((__uint128_t)2 * high * (high + 1) < n) {
        high <<= 1;
        if (high == 0) { /* overflow protection for gigantic n */
            high = UINT64_MAX >> 1;
            break;
        }
    }
    low = high >> 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t tot = (__uint128_t)2 * mid * (mid + 1);
        if (tot >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find the square number (m) that contains the n-th term. */
    idx_t m = find_m(n);

    /* Number of terms contained before this square. */
    __uint128_t prev = (__uint128_t)2 * (m - 1) * m;
    idx_t prev_total = (idx_t)prev;

    /* Offset within the current square, 0‑based. */
    idx_t k = n - prev_total - 1; /* 0 <= k < 4*m */

    /* Compute the x‑coordinate based on which edge k lies on. */
    if (k < m) {
        /* Edge from (m,0) to (0,m), exclude the trailing vertex. */
        return (seq_t)m - (seq_t)k;
    } else if (k < 2 * m) {
        /* Edge from (0,m) to (-m,0). */
        return - (seq_t)(k - m);
    } else if (k < 3 * m) {
        /* Edge from (-m,0) to (0,-m). */
        return - (seq_t)m + (seq_t)(k - 2 * m);
    } else {
        /* Edge from (0,-m) to (m,0), exclude the trailing vertex. */
        return (seq_t)(k - 3 * m);
    }
}
