/*
 * A366055  Expansion of (1/x) * Series_Reversion( x*(1-x)^3/(1-x+x^4) ).
 * https://oeis.org/A366055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(n+1) * Σ_{k=0..⌊n/4⌋} binomial(n+1,k) *
 *            binomial(3*n-3*k+1, n-4*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    unsigned __int128 sum = 0;
    idx_t maxk = n / 4;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term1 = binomial(n + 1, k);

        idx_t n2 = 3 * n - 3 * k + 1;
        idx_t k2 = n - 4 * k;
        seq_t term2 = binomial(n2, k2);

        sum += (unsigned __int128)term1 * (unsigned __int128)term2;
    }

    seq_t denom = (seq_t)(n + 1);
    return (seq_t)(sum / (unsigned __int128)denom);
}
