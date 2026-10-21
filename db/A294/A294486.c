/*
 * A294486  a(n) = binomial(2*n,n) * (2*n+1)^2.
 * https://oeis.org/A294486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n, n) * (2*n+1)^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t bin = binom(2*n, n);
    seq_t tmp = (seq_t)(2*n + 1);
    return bin * tmp * tmp;
}
