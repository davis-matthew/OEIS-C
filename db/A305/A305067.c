/*
 * A305067  a(n) = 282*2^n - 150.
 * https://oeis.org/A305067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 282*2^n - 150
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow_val = pow2(n);          /* 2^n */
    seq_t mul_val = (seq_t)282 * pow_val; /* 282 * 2^n */
    return mul_val - (seq_t)150;      /* subtract 150 */
}
