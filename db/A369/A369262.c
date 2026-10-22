/*
 * A369262  Expansion of (1/x) * Series_Reversion( x * (1-x) / (1+x^2)^3 ).
 * https://oeis.org/A369262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/2)} binomial(3*n+3, k) *
 *            binomial(2*n-2*k, n-2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t c1 = binomial(3 * n + 3, k);
        seq_t c2 = binomial(2 * n - 2 * k, n - 2 * k);
        total += c1 * c2;
    }
    return total / ((seq_t)n + 1);
}
