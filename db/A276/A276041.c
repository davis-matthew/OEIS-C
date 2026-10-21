/*
 * A276041  Exponential convolution of odd numbers (A005408) with themselves.
 * https://oeis.org/A276041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(n^2 + n + 1)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sum = nn * nn + nn + (seq_t)1;
    return pow2(n) * sum;
}
