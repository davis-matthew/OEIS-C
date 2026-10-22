/*
 * A395785  Expansion of (Sum_{k>=0} binomial(3*k+2,k) * x^k)^4.
 * https://oeis.org/A395785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(k+2,2) * binomial(3*n+11,n-k)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* N = 3*n + 11 */
    idx_t N = 3 * n + 11;

    /* Current binomial coefficient C(N, r) where r = n - k */
    idx_t r = n;
    seq_t binom = binomial(N, r);

    /* Current power 2^k */
    seq_t pow2 = 1;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* binomial(k+2, 2) = (k+2)*(k+1)/2 */
        seq_t comb = ((seq_t)(k + 2) * (seq_t)(k + 1)) / 2;

        /* term = 2^k * comb * binom */
        seq_t term = binom * comb;
        term *= pow2;
        sum += term;

        if (k < n)
        {
            /* Update binom to C(N, r-1) using
               C(N, r-1) = C(N, r) * r / (N - r + 1) */
            seq_t num   = (seq_t)r;
            seq_t denom = (seq_t)(N - r + 1);
            binom = binom * num / denom;

            /* Prepare for next iteration */
            --r;
            pow2 = pow2 * 2;
        }
    }

    return sum;
}
