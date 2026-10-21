/*
 * A294520  Numerators of the partial sums of the reciprocals of the
 *          dodecagonal numbers (k + 1)*(5*k + 1) = A051624(k+1), for k >= 0.
 * https://oeis.org/A294520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of V(5,1;n) = Sum_{k=0..n} 1/((k+1)*(5*k+1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Keep the value as a reduced fraction numerator/denominator. */
    seq_t numer = 0;    /* current numerator  */
    seq_t denom = 1;    /* current denominator */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Denominator of the new term: (k+1)*(5*k+1) */
        seq_t k1      = (seq_t)k + 1;
        seq_t termDen = k1 * (((seq_t)5 * k) + 1);

        /* Reduce the fraction addition by the gcd of the current denom
           and the new term's denom.                               */
        seq_t g1          = gcd(denom, termDen);
        seq_t termDen_div = termDen / g1;
        seq_t denom_div   = denom   / g1;

        /* Add the new term:  numer/denom + 1/termDen          */
        seq_t newNumer = numer * termDen_div + denom_div;
        seq_t newDenom = denom * termDen_div;   /* denom/g1 * termDen */

        /* Reduce the result again */
        seq_t g2 = gcd(newNumer, newDenom);
        numer   = newNumer / g2;
        denom   = newDenom / g2;
    }

    return numer;   /* a(n) is the numerator of the reduced sum */
}
