/*
 * A305165  a(n) = 136*2^n - 112.
 * https://oeis.org/A305165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 136*2^n - 112
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 24, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t res = (seq_t)136 * pow2(n);
    res -= (seq_t)112;
    return res;
}
