/*
 * A369483  Expansion of (1/x) * Series_Reversion( x / (1+x+x^3)^2 ).
 * https://oeis.org/A369483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(2*n+2,k) *
 *            binomial(2*n-k+2,n-3*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t k_max = n / 3;
    seq_t sum = 0;

    for (idx_t k = 0; k <= k_max; ++k) {
        seq_t c1 = binomial(2*n + 2, k);
        seq_t c2 = binomial(2*n - k + 2, n - 3*k);
        sum += c1 * c2;
    }

    seq_t denom = (seq_t)n + 1;
    return sum / denom;
}
