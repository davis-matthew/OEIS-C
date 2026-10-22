/*
 * A381374  Little Hankel transform of A317614: a(n) = A317614(n+1)^2 -
 *          A317614(n)*A317614(n+2).
 * https://oeis.org/A381374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10 + 6*(-1)^n + 4*n*(n+2)*(3*(n+1)^2 + (-1)^n*(2*n^2 +
 *            4*n + 5)))/16, but the OEIS offset is 1, so we use n-1 in the
 *            formula
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* safety guard – offset is 1 */
        return (seq_t)0;

    /* use m = n-1 so that the formula matches the OEIS offset */
    seq_t m = (seq_t)(n - 1);
    seq_t m1 = m + 1;          /* (n+1) in the original formula */
    seq_t m2 = m + 2;          /* (n+2) in the original formula */

    /* constants used in the expression */
    const seq_t two   = (seq_t)2;
    const seq_t four  = (seq_t)4;
    const seq_t six   = (seq_t)6;
    const seq_t ten   = (seq_t)10;
    const seq_t sixteen = (seq_t)16;

    /* (-1)^n : from parity of m */
    const int odd = (int)(m & (seq_t)1);

    /* compute the inner expression:
       3*(m+1)^2 + (-1)^n*(2*m^2 + 4*m + 5) */
    seq_t term1 = (seq_t)3 * m1 * m1;
    seq_t term2 = (seq_t)2 * m * m + (seq_t)4 * m + (seq_t)5;
    seq_t inner = odd ? (term1 - term2) : (term1 + term2);

    /* 4*m*(m+2)*inner */
    seq_t mult = four * m * m2 * inner;

    /* 10 + 6*(-1)^n */
    seq_t part = odd ? (ten - six) : (ten + six);

    /* numerator and division by 16 */
    seq_t numerator = part + mult;
    return numerator / sixteen;
}
