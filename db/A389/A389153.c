/*
 * A389153  a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) *
 *          binomial(n+3*k,n-2*k).
 * https://oeis.org/A389153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) *
 *            binomial(n+3*k,n-2*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term1 = binomial(n, k);
        seq_t term2 = binomial(n + 3 * k, n - 2 * k);
        sum += term1 * term2;
    }
    return sum;
}
