/*
 * A357555  a(n) is the numerator of Sum_{d|n} (-1)^(d+1) / d^2.
 * https://oeis.org/A357555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Σ_{d|n} (-1)^(d+1)/d^2
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = 0;   /* current numerator */
    seq_t den = 1;   /* current denominator (in lowest terms) */

    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* first divisor i */
            idx_t d = i;
            seq_t d2 = (seq_t)d * (seq_t)d;
            /* add or subtract the term (-1)^(d+1)/d^2  */
            seq_t new_num = (d & 1) ? (num * d2 + den) : (num * d2 - den);
            seq_t new_den = den * d2;
            seq_t g = gcd(new_num, new_den);
            num = new_num / g;
            den = new_den / g;

            /* second (paired) divisor n/i, if distinct */
            idx_t d3 = n / i;
            if (d3 != d)
            {
                d = d3;
                d2 = (seq_t)d * (seq_t)d;
                new_num = (d & 1) ? (num * d2 + den) : (num * d2 - den);
                new_den = den * d2;
                g = gcd(new_num, new_den);
                num = new_num / g;
                den = new_den / g;
            }
        }
    }

    return num;
}
