/*
 * A293504  The integer k that minimizes |k/n^2 - sqrt(2)|.
 * https://oeis.org/A293504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(1/2 + (n^2)*sqrt(2))
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 81, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Work in seq_t to avoid overflow for the small n that are tested. */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n4 = n2 * n2;          /* n^4 */
    seq_t two_n4 = n4 * 2;       /* 2*n^4 = (n^2*sqrt(2))^2 */
    seq_t s = floor_sqrt(two_n4);/* floor(sqrt(2*n^4)) */

    /* Compare 8*n^4 with (2*s+1)^2 to decide rounding.
       If 8*n^4 < (2*s+1)^2  → round down to s
       else                 → round up to s+1   */
    seq_t left  = n4 * 8;          /* 8*n^4 */
    seq_t t     = s * 2 + 1;       /* 2*s+1 */
    seq_t right = t * t;           /* (2*s+1)^2 */

    if (left < right)
        return s;
    else
        return s + 1;
}
