/*
 * A369400  Expansion of (1/x) * Series_Reversion( x / (1+x)^2 * (1-x^3)^2 ).
 * https://oeis.org/A369400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/3⌋} binomial(2*n+k+1,k) *
 *            binomial(2*n+2,n-3*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t kmax = n / 3;        /* ⌊n/3⌋ */
    seq_t sum  = 0;

    for (idx_t k = 0; k <= kmax; ++k) {
        /* term1 = C(2*n + k + 1, k) */
        seq_t term1 = binomial(2 * n + k + 1, k);
        /* term2 = C(2*n + 2, n - 3*k) */
        seq_t term2 = binomial(2 * n + 2, n - 3 * k);

        /* add product to sum */
        seq_t prod = term1 * term2;
        sum += prod;
    }

    /* a(n) = sum / (n + 1) */
    seq_t result = sum / ((seq_t)n + 1);
    return result;
}
