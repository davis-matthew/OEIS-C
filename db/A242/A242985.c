/*
 * A242985  a(n) = 4^n + 2^(n+1).
 * https://oeis.org/A242985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n + 2^(n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute 4^n */
    seq_t pow4n = pow_uint(4, n);
    /* Compute 2^(n+1) */
    seq_t pow2np1 = pow_uint(2, n + 1);
    return pow4n + pow2np1;
}
