/*
 * A343850  Integer part of the area of an irregular hexagon, formed by the
 *          regular overlap of two regular pentagons each of side length n.
 * https://oeis.org/A343850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2 * n^2 * cos(Pi/10) * (1 + sin(Pi/10)))
 * Range:     u8 n <= 9, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   cmp_sqrt5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Use algebraic manipulation:
    k = 2 * cos(Pi/10) * (1 + sin(Pi/10)) = sqrt( (25 + 11*sqrt5) / 8 )
    a(n) = floor(k * n^2)
          = floor( sqrt( n^4 * (25 + 11*sqrt5) / 8 ) )
    Let m = n^2 and M = m^2 = n^4.
    We need to find the largest integer y such that
          8*y^2 - 25*M <= 11*M*sqrt5
    The right‑hand side is positive, so for negative left side the
    inequality is automatically satisfied.  Testing the sign is done
    by comparing squares:  X <= sqrt5 * T   ↔   X^2 <= 5*T^2   (X > 0).
    The binary search is performed over 0 … 3*m (since k<3). */

#include <stdint.h>
#include "../utils/cmp_sqrt5.h"

/* Sequence function */
seq_t a(idx_t n)
{
    /* n >= 1, use the offset of 1 */
    __int128 N2 = (__int128)n * n;       /* n^2   */
    __int128 N4 = N2 * N2;               /* n^4   */
    __int128 T  = 11 * N4;               /* 11*n^4 */

    seq_t lo = 0;
    seq_t hi = (seq_t)((__int128)3 * N2); /* 3*n^2  upper bound */

    while (lo < hi) {
        seq_t mid = lo + ((hi - lo + 1) >> 1);
        __int128 Y   = (__int128)mid;
        __int128 left = 8 * Y * Y - 25 * N4;   /* 8*y^2 - 25*n^4 */

        if (cmp_sqrt5(left, T) == -1)   /* left <= T*sqrt5 */
            lo = mid;                   /* candidate valid, try higher */
        else
            hi = mid - 1;               /* candidate too high */
    }

    return lo;
}
