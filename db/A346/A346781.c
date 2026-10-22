/*
 * A346781  a(n) is the numerator of the sum of the first n terms of 1 - 1/3 -
 *          1/5 + 1/7 + 1/9 - 1/11 - 1/13 + ... .
 * https://oeis.org/A346781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the numerator of the reduced fraction of S(n) =
 *            Σ_{k=0}^{n-1} s_k/(2k+1) where s_k = +1 if k%4==0 or k%4==3,
 *            otherwise s_k = -1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* initial fraction 0/1 */
    seq_t num = 0;
    seq_t den = 1;

    for (idx_t k = 0; k < n; ++k)
    {
        seq_t term_den = (seq_t)(2 * k + 1);          /* denominator of current term */

        /* new numerator: num*term_den ± den */
        seq_t new_num;
        if ((k % 4ULL == 0ULL) || (k % 4ULL == 3ULL))
            new_num = num * term_den + den;          /* positive sign */
        else
            new_num = num * term_den - den;          /* negative sign */

        seq_t new_den = den * term_den;               /* new denominator */

        /* reduce fraction */
        seq_t g = gcd(new_num, new_den);             /* gcd of positive integers */
        num = new_num / g;
        den = new_den / g;
    }

    return num;                                       /* numerator of a(n) */
}
