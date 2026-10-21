/*
 * A270567  Expansion of g.f. (1+4*x)/(1-5*x).
 * https://oeis.org/A270567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 9 * 5^(n-1) for n > 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t pow = big_pow((seq_t)5, (seq_t)(n - 1));
    return pow * (seq_t)9;
}
