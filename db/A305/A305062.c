/*
 * A305062  a(n) = 96*2^n + 80.
 * https://oeis.org/A305062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 96*2^n + 80
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 25, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);   // compute 2^n as a seq_t
    val *= 96;             // multiply by 96
    val += 80;             // add 80
    return val;
}
