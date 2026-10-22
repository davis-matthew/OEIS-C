/*
 * A397737  Number of assembly trees for the wheel graph W_n.
 * https://oeis.org/A397737
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n-4,n-2)*(1/2 +
 *            hypergeom([1,3,3-n],[5-2*n],1)) Equivalently, with m = n-1: a(n)
 *            = binomial(2*m-2, m-1)/2 + m * Σ_{b=2}^{m} (b! * binomial(2*m-b,
 *            m-b)/(2*m-b)). This implementation uses the latter form and
 *            relies only on the provided binomial() routine. It works for any
 *            seq_t th...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For indices below the offset the sequence is undefined;
       return 0 (any value is acceptable here). */
    if (n < 3) return (seq_t)0;

    /* m = n-1 */
    idx_t m = n - 1;

    /* First part: binomial(2*m-2, m-1) / 2 */
    seq_t base_term = binomial(2*m - 2, m - 1);
    base_term = base_term / (seq_t)2;

    /* Second part: m * Σ_{b=2}^{m} b! * binomial(2*m-b, m-b) / (2*m-b) */
    seq_t sum = 0;
    seq_t fact_b = 1;                     /* holds b! */

    /* Loop over b = 2 .. m */
    for (idx_t b = 2; b <= m; ++b)
    {
        /* Update factorial to b! */
        fact_b = fact_b * (seq_t)b;

        /* Compute binomial(2*m-b, m-b) */
        seq_t binom = binomial(2*m - b, m - b);

        /* Compute term = fact_b * binom / (2*m - b) */
        seq_t denom = (seq_t)(2*m - b);
        seq_t term   = fact_b * binom;
        term = term / denom;

        /* Accumulate */
        sum = sum + term;
    }

    /* Final result */
    seq_t result = base_term + (seq_t)m * sum;
    return result;
}
