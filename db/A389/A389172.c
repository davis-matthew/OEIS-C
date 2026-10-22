/*
 * A389172  a(n) = 2^(n+1) + 4^(n-1) - 1.
 * https://oeis.org/A389172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) + 4^(n-1) - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 in the OEIS offset, so n-1 is valid. */
    seq_t p2 = pow_uint(2, n + 1);
    seq_t p4 = pow_uint(4, n - 1);
    return p2 + p4 - 1;
}
