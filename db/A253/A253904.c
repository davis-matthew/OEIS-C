/*
 * A253904  Denominators of approximation to 2^(1/3) by Halley's method after
 *          n iterations.
 * https://oeis.org/A253904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Halley's method for cube root of 2 after n
 *            iterations. Recurrence: start with p = 1, q = 1 for each i = 1
 *            .. n t1 = p * p * p // p³ t2 = q * q * q // q³ num = p * (t1 + 4
 *            * t2) den = q * (2 * (t1 + t2)) g = seq_gcd(num, den) p = num /
 *            g q = den / g a(n) = q. (q is reduced de...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Starting fraction 1/1 (p=1, q=1) corresponds to a(0)=1. */
    seq_t p = (seq_t)1;
    seq_t q = (seq_t)1;

    /* If n == 0, return the initial denominator. */
    if (n == 0)
        return q;

    /* Iterate from 1 to n. */
    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        /* Compute p³ and q³. */
        seq_t t1 = p * p * p;   /* p³ */
        seq_t t2 = q * q * q;   /* q³ */

        /* Numerator and denominator before reduction. */
        seq_t num = p * (t1 + (seq_t)4 * t2);   /* p * (p³ + 4q³) */
        seq_t den = q * ((seq_t)2 * (t1 + t2)); /* q * 2(p³ + q³) */

        /* Reduce the fraction. */
        seq_t g = seq_gcd(num, den);
        if (g != (seq_t)1)
        {
            num /= g;
            den /= g;
        }

        /* Update for next iteration. */
        p = num;
        q = den;
    }

    return q;
}
