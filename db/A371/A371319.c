/*
 * A371319  E.g.f. satisfies A(x) = exp(x) + x^2*A(x)^2.
 * https://oeis.org/A371319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/2⌋} (k+1)^(n-2k-1) *
 *            binomial(2k,k)/(n-2k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t maxk = n >> 1;       /* floor(n/2) */
    seq_t sum = 0;

    idx_t m = n;                /* current value of n-2k */
    seq_t fact_ratio = 1;       /* n!/(m)!  initially 1 */
    seq_t binom = 1;            /* binomial(2k,k)  initially 1 */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t base = (seq_t)(k + 1);

        if (m == 0)            /* exponent = -1: term = fact_ratio * binom / base */
        {
            /* guarantee exact division */
            seq_t term = fact_ratio * binom / base;
            sum += term;
        }
        else                    /* m >= 1 */
        {
            seq_t pow_term = big_pow(base, m - 1);
            seq_t term = fact_ratio * pow_term * binom;
            sum += term;
        }

        /* prepare for next k */
        if (k < maxk)
        {
            /* fact_ratio_{k+1} = fact_ratio_k * m * (m-1)   (since m = n-2k) */
            fact_ratio *= m;
            fact_ratio *= (m - 1);

            /* binom update:
             *  binom_{k+1} = binom_k * (2k+1)(2k+2) / (k+1)^2
             */
            idx_t kk = k + 1;
            seq_t num = (seq_t)(2 * kk - 1) * (seq_t)(2 * kk); /* (2k+1)(2k+2) */
            seq_t den = (seq_t)kk * (seq_t)kk;                 /* (k+1)^2 */
            binom = binom * num / den;

            m -= 2;   /* next m = n-2(k+1) */
        }
    }

    return sum;
}
