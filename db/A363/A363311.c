/*
 * A363311  Expansion of g.f. A(x) satisfying A(x) = 1 + x*(A(x)^3 + A(x)^5).
 * https://oeis.org/A363311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊(n-1)/2⌋} 2^{n-k} * binomial(n,k) *
 *            binomial(4*n-k, n-1-2*k) / n (OEIS A363311)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t max_k = (n - 1) / 2;
    seq_t sum = 0;
    seq_t bin1 = 1;                     /* binomial(n,0) */

    for (idx_t k = 0; k <= max_k; ++k)
    {
        /* binomial(4*n-k, n-1-2*k) via helper */
        seq_t bin2 = binomial(4 * n - k, n - 1 - 2 * k);

        /* 2^{n-k} */
        seq_t pw2 = pow2(n - k);

        /* term = pw2 * bin1 * bin2 */
        seq_t term = pw2;
        term *= bin1;
        term *= bin2;

        sum += term;

        /* update bin1 for next k: binomial(n,k+1) = bin1 * (n-k)/(k+1) */
        if (k != max_k)          /* avoid unused division after last iteration */
        {
            bin1 *= (seq_t)(n - k);
            bin1 /= (seq_t)(k + 1);
        }
    }

    /* division by n (exact) */
    return sum / (seq_t)n;
}
