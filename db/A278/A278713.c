/*
 * A278713  Numerators of (n-1)*(n-3)/(6*(2*n-1)); equivalently, numerators of
 *          Dedekind sum s(2,2*n-1).
 * https://oeis.org/A278713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(n) = numerator of (n-1)*(n-3)/(6*(2n-1))
 * Range:     i8 n <= 11, i16 n <= 192, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute numerator and denominator */
    seq_t nn = (seq_t)n;
    seq_t num = (nn - 1) * (nn - 3);          /* (n-1)*(n-3) may be negative */
    seq_t den = 6 * (2 * nn - 1);             /* 6*(2n-1), always positive */

    /* Record sign of numerator */
    int sign = 1;
    if (num < 0) {
        sign = -1;
        num = -num;                          /* now num is non‑negative */
    }

    /* Reduce the fraction: compute the GCD of numerator and denominator */
    seq_t g = gcd(num, den);

    /* Return the signed reduced numerator */
    return (seq_t)(sign * (num / g));
}
