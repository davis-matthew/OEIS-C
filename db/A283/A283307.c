/*
 * A283307  List points (x,y) having integer coordinates, sorted first by
 *          x^2+y^2 and in case of ties, by x-coordinate and then by
 *          y-coordinate. Sequence gives x-coordinates.
 * https://oeis.org/A283307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Enumerate all integer lattice points (x,y) sorted first by
   increasing x²+y², and for equal distance by increasing x, then increasing y.
   Return the x‑coordinate of the nth point (a(1)=0). */
/* This implementation uses only integer arithmetic and works for any
   seq_t/int types. */

#include <stdint.h>

static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32); /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Special case for n == 1 (the origin). */
    if (n == (idx_t)1) return (seq_t)0;

    idx_t count = 0;               /* number of points produced so far */

    for (idx_t sum = 0; ; ++sum) {              /* sum = x² + y² */
        idx_t maxx = isqrt_u64(sum);            /* |x| <= maxx */
        for (int64_t x = -(int64_t)maxx; x <= (int64_t)maxx; ++x) {
            __int128 xx = (__int128)x * x;
            if (xx > sum) continue;             /* should not happen */
            idx_t t = sum - (idx_t)xx;          /* y² value */

            idx_t y = isqrt_u64(t);
            if ((unsigned __int128)y * y != t) continue;  /* not a perfect square */

            if (y == 0) {                     /* only one point (x,0) */
                ++count;
                if (count == n) return (seq_t)x;
            } else {
                /* two points: (x,-y) then (x,y) */
                ++count;
                if (count == n) return (seq_t)x;
                ++count;
                if (count == n) return (seq_t)x;
            }
        }
    }
}
