/*
 * A305261  a(n) = 120*2^n - 108.
 * https://oeis.org/A305261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 120*2^n - 108
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);
    val = seq_mul(val, (seq_t)120);
    val -= (seq_t)108;
    return val;
}
