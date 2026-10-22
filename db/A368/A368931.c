/*
 * A368931  Expansion of (1/x) * Series_Reversion( x * (1-x) * (1-x-x^3) ).
 * https://oeis.org/A368931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(n+k,k) *
 *            binomial(3n-2k+1,n-3k)
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
    idx_t max_k = n / 3;
    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t b1 = binomial(n + k, k);
        seq_t b2 = binomial(3 * n - 2 * k + 1, n - 3 * k);
        sum += (unsigned __int128)b1 * (unsigned __int128)b2;
    }
    unsigned __int128 denom = (unsigned __int128)(n + 1);
    return (seq_t)(sum / denom);
}
