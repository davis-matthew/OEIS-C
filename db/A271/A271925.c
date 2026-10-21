/*
 * A271925  Numerator of (Product_{j=0..n-1}
 *          (((2*j+1)*(3*j+4))/((j+1)*(6*j+1))) - 1).
 * https://oeis.org/A271925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = numerator of ( Product_{j=0}^{n-1} (((2*j+1)*(3*j+4))/((j+1)*(6*j+1))) - 1 )
 * in lowest terms.
 * The computation is done by maintaining the product as a reduced fraction
 * (num/den).  At each step the current factor is reduced together with
 * the existing numerator and denominator to keep intermediate values small.
 * After the product is formed, 1 is subtracted, the result is reduced again,
 * and the numerator is returned.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* undefined for n < 1; return 0 for safety */
    if (n < 1) {
        return 0;
    }

    seq_t num = 1;   /* numerator of the product */
    seq_t den = 1;   /* denominator of the product */

    for (idx_t k = 0; k < n; ++k) {
        seq_t j = (seq_t)k;

        /* factor f_j = ((2*j+1)*(3*j+4)) / ((j+1)*(6*j+1)) */
        seq_t fnum = (j * (seq_t)2 + 1) * (j * (seq_t)3 + 4);
        seq_t fden = (j + 1) * (j * (seq_t)6 + 1);

        /* reduce factor itself */
        seq_t g0 = gcd(fnum, fden);
        if (g0 > 1) {
            fnum /= g0;
            fden /= g0;
        }

        /* cross‑reduce with current fraction to keep things small */
        seq_t g1 = gcd(num, fden);
        if (g1 > 1) {
            num /= g1;
            fden /= g1;
        }
        seq_t g2 = gcd(den, fnum);
        if (g2 > 1) {
            den /= g2;
            fnum /= g2;
        }

        /* multiply and reduce */
        num = num * fnum;
        den = den * fden;

        seq_t g3 = gcd(num, den);
        if (g3 > 1) {
            num /= g3;
            den /= g3;
        }
    }

    /* subtract 1: (num/den) - 1 = (num - den)/den */
    seq_t new_num = num - den;
    seq_t new_den = den;

    seq_t g4 = gcd(new_num, new_den);
    if (g4 > 1) {
        new_num /= g4;
        /* new_den /= g4;  // not needed after this point */
    }

    return new_num;
}
