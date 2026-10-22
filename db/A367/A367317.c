/*
 * A367317  Expansion of (1/x) * Series_Reversion( x * (1-x-x^4/(1-x)) ).
 * https://oeis.org/A367317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/4⌋} binomial(n+k,k) *
 *            binomial(2*n-2*k, n-4*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External helper routine: binomial is provided in ../utils/binomial.h */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term1 = binomial(n + k, k);
        seq_t term2 = binomial(2 * n - 2 * k, n - 4 * k);
        sum += term1 * term2;
    }
    return sum / ((seq_t)n + 1);
}
