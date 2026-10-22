/*
 * A369402  Expansion of (1/x) * Series_Reversion( x / (1+x)^2 * (1-x^3)^3 ).
 * https://oeis.org/A369402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} binomial(3*n+k+2, k) *
 *            binomial(2*n+2, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = n / 3;
    for (idx_t k = 0; k <= kmax; ++k)
    {
        seq_t t1 = binomial(3 * n + k + 2, k);
        seq_t t2 = binomial(2 * n + 2, n - 3 * k);
        seq_t prod = seq_mul(t1, t2);
        sum += prod;
    }
    return sum / (seq_t)(n + 1);
}
