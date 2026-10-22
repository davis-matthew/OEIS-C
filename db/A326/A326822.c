/*
 * A326822  T(n, k) = k^0 if k = 1 else 0^n. Triangle read by rows, T(n, k)
 *          for 0 <= k <= n.
 * https://oeis.org/A326822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if n==0 or n>1 and (n-1) is a triangular number;
 *            otherwise 0
 * Range:     u8 n <= 8, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* first term */
        return 1;
    if (n == 1)            /* second term is 0 */
        return 0;

    /* d = 8*n - 7 ; discriminant of m^2 + m - 2*(n-1) = 0 */
    seq_t d = (seq_t)8 * (seq_t)n - (seq_t)7;
    seq_t s = floor_sqrt(d);

    /* if d is a perfect square and its square root is odd, then n-1 is triangular */
    if (s * s == d && (s & (seq_t)1))
        return 1;
    return 0;
}
