/*
 * A351697  32*a(n) is the denominator of the squared circumradius of a cyclic
 *          quadrilateral with sides n, n+1, n+2, n+3.
 * https://oeis.org/A351697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (8*(2*n^6 + 18*n^5 + 65*n^4 + 120*n^3 +
 *            117*n^2 + 54*n + 9))/(n^4 + 6*n^3 + 11*n^2 + 6*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 23, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include gcd helper from utils */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t type for arithmetic */
    seq_t sn = (seq_t)n;
    seq_t sn2 = sn * sn;            /* n^2 */
    seq_t sn3 = sn2 * sn;           /* n^3 */
    seq_t sn4 = sn3 * sn;           /* n^4 */
    seq_t sn5 = sn4 * sn;           /* n^5 */
    seq_t sn6 = sn5 * sn;           /* n^6 */

    /* Numerator: 8 * (2*n^6 + 18*n^5 + 65*n^4 + 120*n^3 + 117*n^2 + 54*n + 9) */
    seq_t num = 8 * ( (2 * sn6) + (18 * sn5) + (65 * sn4) + (120 * sn3)
                     + (117 * sn2) + (54 * sn) + 9 );

    /* Denominator: n^4 + 6*n^3 + 11*n^2 + 6*n */
    seq_t den = sn4 + (6 * sn3) + (11 * sn2) + (6 * sn);

    /* Reduce the fraction by the greatest common divisor */
    seq_t g = gcd(num, den);

    /* Return the denominator in lowest terms */
    return den / g;
}
