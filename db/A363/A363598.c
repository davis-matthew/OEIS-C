/*
 * A363598  Expansion of Sum_{k>0} x^(2*k)/(1+x^k)^4.
 * https://oeis.org/A363598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^d * binomial(d+1,3)
 * Range:     i8 n <= 9, i16 n <= 55, i32 n <= 1857, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for all n >= 1. */

#include <stdbool.h> /* Only for bool if needed; not used here but allowed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* not part of the sequence, but safe */
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* divisor d1 = i */
            idx_t d1 = i;
            seq_t bd1 = (seq_t)d1 + 1;
            seq_t t1 = bd1 * (seq_t)d1 * (seq_t)(d1 - 1) / 6;
            seq_t sign1 = ((d1 & 1) ? (seq_t)-1 : (seq_t)1);
            sum += sign1 * t1;

            /* divisor d2 = n / i , if distinct */
            idx_t d2 = n / i;
            if (d2 != d1)
            {
                seq_t bd2 = (seq_t)d2 + 1;
                seq_t t2 = bd2 * (seq_t)d2 * (seq_t)(d2 - 1) / 6;
                seq_t sign2 = ((d2 & 1) ? (seq_t)-1 : (seq_t)1);
                sum += sign2 * t2;
            }
        }
    }
    return sum;
}
