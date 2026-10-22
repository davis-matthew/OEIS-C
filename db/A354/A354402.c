/*
 * A354402  a(n) is the numerator of Sum_{k=1..n} (-1)^(k+1) / (k*k!).
 * https://oeis.org/A354402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the numerator of Sum_{k=1..n} (-1)^(k+1) / (k*k!)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = 0;          /* numerator of partial sum */
    seq_t den = 1;          /* denominator of partial sum (reduced) */
    seq_t fact = 1;         /* current factorial k! */

    for (idx_t k = 1; k <= n; ++k) {
        /* update factorial: fact = k!          */
        fact *= (seq_t)k;

        /* term denominator = k*k!              */
        seq_t term_den = fact;
        term_den *= (seq_t)k;

        /* combine fractions with reduction          */
        seq_t g = gcd(den, term_den);          /* gcd of denominators          */
        seq_t lcm_mult = term_den / g;         /* factor by which den is scaled */
        seq_t term_factor = den / g;           /* contribution of term's numerator */

        /* compute new numerator: num * lcm_mult +/- term_factor */
        seq_t new_num = num * lcm_mult;
        if ((k & 1) == 0)                      /* even k ⇒ negative term     */
            new_num -= term_factor;
        else
            new_num += term_factor;

        /* new denominator */
        seq_t new_den = den * lcm_mult;

        /* reduce fraction a/b to lowest terms          */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return num;
}
