/*
 * A398807  Expansion of (Sum_{k>=0} binomial(5*k+1,k) * x^k)^4.
 * https://oeis.org/A398807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 4^k * binomial(k+2,2) * binomial(5*n+7,n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    const idx_t N = 5 * n + 7;                  /* N = 5*n + 7                */
    seq_t result = 0;                           /* sum accumulator            */
    seq_t pow4   = 1;                           /* 4^k                         */
    seq_t binom_cur = binomial(N, n);           /* binom(N, n)                 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* coeff = C(k+2,2) = (k+2)*(k+1)/2 */
        seq_t coeff = ((seq_t)(k + 2) * (seq_t)(k + 1)) / 2;

        /* term = binom_cur * coeff * pow4 */
        seq_t term = binom_cur;
        term *= coeff;
        term *= pow4;

        result += term;

        if (k == n) break; /* avoid one more update after last iteration */

        /* update for next k: multiply pow4 by 4 */
        pow4 *= 4;

        /* update binom_cur from binom(N, m) to binom(N, m-1)
           where m = n - k (current)               */
        idx_t m = n - k;
        seq_t numerator   = m;
        seq_t denominator = N - m + 1;   /* N - (m-1) */
        binom_cur = binom_cur * numerator / denominator;
    }

    return result;
}
