/*
 * A328184  Denominator of time taken for a vertex of a rolling regular
 *          n-sided polygon to reach the ground.
 * https://oeis.org/A328184
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((n - 1)/(2*n)) for n even; a(n) =
 *            denominator((2*n - 3)/(4*n)) for n odd
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0; /* undefined below offset, any value is acceptable */

    seq_t ns = (seq_t)n;
    seq_t num, den;

    if ((n & 1) == 0) {          /* even n */
        num = ns - 1;            /* n - 1 */
        den = ns * 2;            /* 2*n */
    } else {                     /* odd n */
        num = ns * 2 - 3;        /* 2*n - 3 */
        den = ns * 4;            /* 4*n */
    }

    seq_t g = gcd(num, den);
    return den / g;
}
