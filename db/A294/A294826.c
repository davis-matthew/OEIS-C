/*
 * A294826  Numerators of the partial sums of the reciprocals of twice the
 *          heptagonal numbers (k + 1)*(5*k + 2) = A135706(k+1) =
 *          2*A000566(k+1), for k >= 0.
 * https://oeis.org/A294826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of Sum_{k=0..n} 1/((k+1)*(5*k+2))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The code evaluates the rational sum iteratively, reducing after
    each addition to keep the numbers from growing too large.  The
    reduction is performed with the gcd function provided in
    "../utils/gcd.h".  This implementation works for any build
    that supplies the integer type seq_t (fixed width or
    arbitrary-precision) and the integer type idx_t. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* V(5,2;0) = 1/2  */
        return 1;

    seq_t num = 1;            /* current numerator   */
    seq_t den = 2;            /* current denominator */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kk = (seq_t)k;
        seq_t term_den = (kk + 1) * (5 * kk + 2);   /* 1/((k+1)*(5*k+2)) */

        /* Reduce the addition a/b + 1/d */
        seq_t g = gcd(den, term_den);
        seq_t b1 = den / g;          /* b/g */
        seq_t d1 = term_den / g;     /* d/g */

        seq_t new_num = num * d1 + b1;           /* a*(d/g) + 1*(b/g) */
        seq_t new_den = b1 * term_den;           /* lcm(b,d) = b/g * d */

        /* Final reduction */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return num;
}
