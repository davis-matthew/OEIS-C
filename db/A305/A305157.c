/*
 * A305157  a(n) = 164*2^n - 99.
 * https://oeis.org/A305157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 164*2^n - 99
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    return (seq_t)164 * pow2(n) - (seq_t)99;
}
