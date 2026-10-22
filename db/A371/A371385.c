/*
 * A371385  Expansion of (1/x) * Series_Reversion( x * (1-3*x)^3 / (1-x) ).
 * https://oeis.org/A371385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..n} 2^k * binomial(3*n+k+2,k) * *
 *            binomial(3*n+1,n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(0) == 1, so the loop works for n = 0 as well. */
    seq_t sum = 0;
    seq_t pow2_term = 1;            /* 2^k  */
    seq_t bin2 = 1;                 /* C(3n+2,0)   */
    seq_t bin1 = binomial(3*n + 1, n);  /* C(3n+1,n)   */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = 2^k * bin2 * bin1 */
        seq_t term = pow2_term;
        term = term * bin2;
        term = term * bin1;
        sum += term;

        if (k == n) break;  /* last iteration, nothing to update */

        /* Update for next k */
        pow2_term <<= 1;  /* multiply by 2 */

        /* bin2_{k+1} = bin2_k * (3n+k+3) / (k+1) */
        seq_t tmp = bin2 * (seq_t)(3*n + k + 3);
        bin2 = tmp / (k + 1);

        /* bin1_{k+1} = bin1_k * (n-k) / (2n+k+2) */
        seq_t tmp1 = bin1 * (seq_t)(n - k);
        bin1 = tmp1 / ((seq_t)(2*n + k + 2));
    }

    return sum / (n + 1);
}
