/*
 * A369161  Expansion of (1/x) * Series_Reversion( x * ((1-x)^3-x^4) ).
 * https://oeis.org/A369161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(n+1) * Σ_{k=0..⌊n/4⌋} binomial(n+k, k) *
 *            binomial(4n-k+2, n-4k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_u128.h"

seq_t a(idx_t n)
{
    unsigned __int128 sum = 0;
    idx_t max_k = n / 4;
    for (idx_t k = 0; k <= max_k; ++k)
    {
        unsigned __int128 b1 = binom_u128(n + k, k);
        unsigned __int128 b2 = binom_u128(4 * n - k + 2, n - 4 * k);
        sum += b1 * b2;
    }
    unsigned __int128 result = sum / (unsigned __int128)(n + 1);
    return (seq_t)result;
}
