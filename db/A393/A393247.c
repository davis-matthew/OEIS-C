/*
 * A393247  a(n) = Sum_{k=0..floor(n/5)} binomial(2*k,k) *
 *          binomial(n-2*k,3*k).
 * https://oeis.org/A393247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/5)} binom(2*k,k) * binom(n-2*k,3*k)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 5;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t c1 = binom(2 * k, k);
        seq_t c2 = binom(n - 2 * k, 3 * k);
        sum += c1 * c2;
    }
    return sum;
}
