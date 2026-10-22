/*
 * A389128  a(n) = Sum_{k=0..floor(n/4)} binomial(n,k) * binomial(n+k,n-4*k).
 * https://oeis.org/A389128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(n,k)*binomial(n+k,n-4*k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 79,
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
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term1 = binomial(n, k);
        seq_t term2 = binomial(n + k, n - 4 * k);
        sum += term1 * term2;
    }
    return sum;
}
