/*
 * A389986  E.g.f. A(x) satisfies A(x) = exp(x * (1+x^2)^3 * A(x)).
 * https://oeis.org/A389986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A389986
   Formula: a(n) = n! * Σ_{k=0..⌊n/2⌋} (n-2k+1)^(n-2k-1) * binomial(3·(n-2k), k) / (n-2k)! 
   Implementation uses iterative factorial ratio to avoid repeated factorials
   and pow_seq for integer powers (exponent ≤ 9999 for n ≤ 10000). */

#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* Start with k = 0, m = n, and fact_ratio = n! / m! = 1. */
    idx_t m = n;
    seq_t fact_ratio = 1;

    idx_t maxk = n / 2;        /* floor(n/2) */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* Compute (m+1)^(m-1); for m <= 1 the value is 1. */
        seq_t power_val;
        if (m <= 1)
            power_val = 1;
        else
            power_val = pow_seq((seq_t)(m + 1), (int)(m - 1));

        /* binomial(3m, k) */
        seq_t bin_val = binomial(3 * m, k);

        /* term = fact_ratio * power_val * bin_val */
        seq_t term = fact_ratio;
        term *= power_val;
        term *= bin_val;

        sum += term;

        /* Update fact_ratio and m for next k (if any).  
           fact_ratio = n! / (m-2)! = fact_ratio * m * (m-1).
           Then m decreases by 2. */
        if (k < maxk) {
            fact_ratio *= (seq_t)m;
            fact_ratio *= (seq_t)(m - 1);
            m -= 2;
        }
    }

    return sum;
}
