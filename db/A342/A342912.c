/*
 * A342912  a(n) = [x^n] (1 - 2*x - sqrt((1 - 3*x)/(1 + x)))/(2*x^3).
 * https://oeis.org/A342912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^n] (1 - 2*x - sqrt((1 - 3*x)/(1 + x)))/(2*x^3)
 *            Recurrence: a(0)=1, a(1)=1, a(2)=3; for n≥3 a(n) =
 *            (2*a(n-1)+3*a(n-2))*(n+1)/(n+3)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    seq_t a_prevprev = (seq_t)1;  /* a(1) */
    seq_t a_prev     = (seq_t)3;  /* a(2) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        /* numerator part before division */
        seq_t t   = (seq_t)2 * a_prev + (seq_t)3 * a_prevprev;
        seq_t den = (seq_t)(i + 3);

        /* cancel common factors with denominator to avoid overflow */
        seq_t g1 = gcd(t, den);
        t   /= g1;
        den /= g1;

        seq_t g2 = gcd((seq_t)(i + 1), den);
        seq_t mul = ((seq_t)(i + 1)) / g2;
        den /= g2;

        seq_t g3 = gcd(t, den);
        t   /= g3;
        den /= g3;

        seq_t g4 = gcd(mul, den);
        mul /= g4;
        den /= g4;

        next = t * mul;
        if (den != (seq_t)1)
            next /= den;

        a_prevprev = a_prev;
        a_prev     = next;
    }

    return a_prev;
}
