/*
 * A383526  a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) * binomial(n-2*k,k)^2.
 * https://oeis.org/A383526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) * binomial(n-2*k,k)^2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t bk  = binomial(n, k);
        seq_t bnk = binomial(n - 2 * k, k);
        seq_t term = bk * bnk * bnk;
        sum += term;
    }
    return sum;
}
