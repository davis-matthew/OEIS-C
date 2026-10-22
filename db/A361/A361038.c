/*
 * A361038  a(n) = 1680 * (3*n)!/((2*n)!*(n+3)!).
 * https://oeis.org/A361038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = 1680 * (3*n)! / ((2*n)! * (n+3)!)
   Recurrence: a(0)=280,
               a(n)=a(n-1)*((3n-2)(3n-1)(3n))/((2n-1)(2n)(n+3)) for n≥1  */
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)280;

    seq_t res = (seq_t)280;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* build numerator and denominator factors */
        seq_t num1 = (seq_t)(3 * i - 2);
        seq_t num2 = (seq_t)(3 * i - 1);
        seq_t num3 = (seq_t)(3 * i);
        seq_t den1 = (seq_t)(2 * i - 1);
        seq_t den2 = (seq_t)(2 * i);
        seq_t den3 = (seq_t)(i + 3);

        /* Reduce denominators with current result */
        seq_t g;
        g = gcd(res, den1);
        if (g > 1) { res /= g; den1 /= g; }
        g = gcd(res, den2);
        if (g > 1) { res /= g; den2 /= g; }
        g = gcd(res, den3);
        if (g > 1) { res /= g; den3 /= g; }

        /* Reduce numerators with remaining denominators */
        g = gcd(num1, den1);
        if (g > 1) { num1 /= g; den1 /= g; }
        g = gcd(num1, den2);
        if (g > 1) { num1 /= g; den2 /= g; }
        g = gcd(num1, den3);
        if (g > 1) { num1 /= g; den3 /= g; }

        g = gcd(num2, den1);
        if (g > 1) { num2 /= g; den1 /= g; }
        g = gcd(num2, den2);
        if (g > 1) { num2 /= g; den2 /= g; }
        g = gcd(num2, den3);
        if (g > 1) { num2 /= g; den3 /= g; }

        g = gcd(num3, den1);
        if (g > 1) { num3 /= g; den1 /= g; }
        g = gcd(num3, den2);
        if (g > 1) { num3 /= g; den2 /= g; }
        g = gcd(num3, den3);
        if (g > 1) { num3 /= g; den3 /= g; }

        /* multiply by remaining numerators */
        res = res * num1;
        res = res * num2;
        res = res * num3;

        /* divide by remaining denominators (exact division) */
        if (den1 > 1) res /= den1;
        if (den2 > 1) res /= den2;
        if (den3 > 1) res /= den3;
    }
    return res;
}
