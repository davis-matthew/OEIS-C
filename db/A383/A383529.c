/*
 * A383529  a(n) = Sum_{k=0..floor(n/4)} binomial(n+k,k) * binomial(n-3*k,k).
 * https://oeis.org/A383529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(n+k,k) * binomial(n-3*k,k)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term1 = binomial(n + k, k);
        seq_t term2 = binomial(n - 3 * k, k);
        result += term1 * term2;
    }
    return result;
}
