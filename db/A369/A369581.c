/*
 * A369581  Expansion of (1/x) * Series_Reversion( x / (1+x+x^4/(1+x)^2) ).
 * https://oeis.org/A369581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/4)} binomial(n+1,k) *
 *            binomial(n-3*k+1,n-4*k)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t c1 = binomial(n + 1, k);
        seq_t c2 = binomial(n - 3 * k + 1, n - 4 * k);
        sum += c1 * c2;
    }
    seq_t denom = (seq_t)(n + 1);
    return sum / denom;
}
