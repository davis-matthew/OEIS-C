/*
 * A380780  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-x * (1 +
 *          x)) / (1 + x)^2 ).
 * https://oeis.org/A380780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n+1)^(k-1) * binomial(2*n+k+2, n-k) /
 *            k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* n! - required for all terms */
    seq_t fact_n = factorial(n);

    /* A0 = binomial(2*n+2, n)   (computed multiplicatively) */
    seq_t A = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        A *= (seq_t)(n + i + 2);   /* multiply by numerator term */
        A /= (seq_t)i;             /* divide by denominator term */
    }

    /* Term for k = 0 :  n! * binom(2*n+2, n) / (n+1) */
    seq_t term0 = (fact_n * A) / ((seq_t)n + 1);
    seq_t sum = term0;

    /* Variables for k >= 1 */
    seq_t pow_n1 = (seq_t)1;       /* (n+1)^(k-1) */
    seq_t fact_div = fact_n;       /* n!/(k!)   (starts with k = 1) */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update A from binomial(2*n+k+2, n-k) to
           binomial(2*n+k+1, n-(k-1)) using the ratio:
                 A_k = A_{k-1} * (2n+k+2) * (n-k+1) /
                       [(n+2k+2)*(n+2k+1)]                     */
        seq_t N      = (seq_t)(2*n + k + 2);
        seq_t R1     = (seq_t)(n - k + 1);
        seq_t denom1 = (seq_t)(n + 2*k + 2);
        seq_t denom2 = (seq_t)(n + 2*k + 1);
        A = (A * N * R1) / (denom1 * denom2);

        /* Term for this k:
           T_k = (n! / k!) * binomial * (n+1)^(k-1)  (all integers)
           fact_div currently equals n! / k!          */
        seq_t term = fact_div * A * pow_n1;
        sum += term;

        /* Prepare for next iteration */
        pow_n1 *= (seq_t)(n + 1);
        if (k < n)
            fact_div /= (seq_t)(k + 1);   /* next fact_div = n!/(k+1)! */
    }

    return sum;
}
