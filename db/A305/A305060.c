/*
 * A305060  a(n) = 18*2^n + 10.
 * https://oeis.org/A305060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*2^n + 10
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    return (seq_t)18 * pow2(n) + (seq_t)10;
}
