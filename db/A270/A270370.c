/*
 * A270370  a(n) = Sum_{k=0..n} (-1)^k*floor(k^(1/3)).
 * https://oeis.org/A270370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor(n^(1/3))*(-1)^n/2 - ((-1)^(floor(n^(1/3))+1)+1)/4
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer cube root: floor(cbrt(n)) for non‑negative n */
static inline idx_t
floor_cube_root(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* Grow hi until hi^3 > n */
    while ((__int128)hi * hi * hi <= n)
        hi <<= 1;
    /* Binary search between lo and hi */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((__int128)mid * mid * mid <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t
a(idx_t n)
{
    /* r = floor(cbrt(n)) */
    idx_t r = floor_cube_root(n);
    /* s = (-1)^n */
    seq_t sign_n = ((n & 1) == 0) ? 1 : -1;
    /* t = (-1)^(r+1) */
    seq_t sign_rplus1 = (((r + 1) & 1) == 0) ? 1 : -1;

    /* a(n) = (2 * r * sign_n - (sign_rplus1 + 1)) / 4  */
    seq_t numerator = (seq_t)2 * (seq_t)r * sign_n - (sign_rplus1 + 1);
    return numerator / 4;
}
