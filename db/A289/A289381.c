/*
 * A289381  a(n) = numerator of Sum_{k=1..n} 1/(2*k-1)!!.
 * https://oeis.org/A289381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of sum_{k=1..n} 1/(2*k-1)!! (double factorial)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"   /* gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* defined only for n ≥ 1 */

    seq_t sum_num = (seq_t)0;   /* numerator of the partial sum */
    seq_t sum_den = (seq_t)1;   /* denominator of the partial sum */
    seq_t d       = (seq_t)1;   /* (2*k-1)!! for the current k   */

    for (idx_t k = (idx_t)1; k <= n; ++k)
    {
        if (k > (idx_t)1)
            d = d * (seq_t)(2 * k - (idx_t)1);   /* update double factorial */

        /* add 1/d to sum_num/sum_den */
        seq_t g  = gcd(sum_den, d);                 /* gcd of denominators */
        seq_t part_den  = sum_den / g;              /* sum_den / g       */
        seq_t part_term = d / g;                    /* d / g             */

        seq_t num = sum_num * part_term + part_den; /* new numerator      */
        seq_t den = part_den * d;                   /* new denominator    */

        seq_t g2 = gcd(num, den);                   /* reduce fraction    */
        sum_num = num / g2;
        sum_den = den / g2;
    }

    return sum_num;   /* numerator of the reduced sum */
}
