/*
 * A394916  Expansion of (Sum_{k>=0} binomial(5*k,k) * x^k)^3.
 * https://oeis.org/A394916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (k+1) * 4^k * binomial(5*n+2, n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses recurrence:
 * binomial(m,r-1) = binomial(m,r) * r / (m - r + 1)
 * and increases the power of 4 each step.
 * Requires the helper function binom from ../utils/binom.h.
 */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* m = 5*n + 2 */
    idx_t m = 5 * n + 2;

    seq_t sum  = 0;          /* accumulated result */
    seq_t pow4 = 1;          /* 4^k for current k; starts with 4^0 */
    seq_t bin  = binom(m, n);/* binomial(m, n) for k = 0 */
    idx_t  r   = n;          /* current r = n - k */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = pow4 * bin * (seq_t)(k + 1);
        sum += term;

        if (k == n) break;  /* no further terms */

        /* Update bin for next r (r-1) using the recurrence */
        seq_t mult  = (seq_t)r;                     /* r = n - k   */
        seq_t denom = (seq_t)(m - r + 1);           /* m - r + 1   */
        bin = (bin * mult) / denom;
        --r;                                        /* prepare for next iteration */

        pow4 *= 4;                                 /* 4^{k+1}     */
    }

    return sum;
}
