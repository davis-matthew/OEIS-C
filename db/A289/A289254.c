/*
 * A289254  a(n) = 4^n - 3*n - 1.
 * https://oeis.org/A289254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 3*n - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t pow4 = pow_uint(4, n);
    pow4 -= (seq_t)3 * (seq_t)n;
    pow4 -= 1;
    return pow4;
}
