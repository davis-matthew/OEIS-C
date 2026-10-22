/*
 * A354331  a(n) is the denominator of Sum_{k=0..n} 1 / (2*k+1)!.
 * https://oeis.org/A354331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the denominator of the sum \sum_{k=0}^n 1/(2k+1)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"   /* for gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    seq_t num = 0;   /* current numerator of the sum */
    seq_t den = 1;   /* current denominator of the sum, already reduced */
    seq_t fact = 1;  /* (2*k+1)! for the current k */

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k > 0)  /* update factorial from (2*(k-1)+1)! to (2k+1)! */
        {
            fact *= (seq_t)(2 * k);          /* multiply by 2k   */
            fact *= (seq_t)(2 * k + 1);      /* multiply by 2k+1 */
        }

        /* Add the new term 1/fact to the running sum num/den */
        seq_t g1 = gcd(den, fact);               /* gcd(den, fact) */
        seq_t lcm = (den / g1) * fact;           /* lcm(den, fact) */

        /* New numerator: num * (fact/g1) + den/g1 */
        seq_t new_num = num * (fact / g1) + den / g1;
        seq_t g2 = gcd(new_num, lcm);

        num = new_num / g2;
        den = lcm / g2;
    }

    return den;   /* the denominator of the sum */
}
