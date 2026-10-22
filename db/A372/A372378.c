/*
 * A372378  Expansion of (1/x) * Series_Reversion( x / (1+x+x^3)^3 ).
 * https://oeis.org/A372378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} binom(3*n+3, k) *
 *            binom(3*n-k+3, n-3*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t b1 = binom(3 * n + 3, k);
        seq_t b2 = binom(3 * n - k + 3, n - 3 * k);
        sum += b1 * b2;
    }

    return sum / ((seq_t)n + 1);
}
