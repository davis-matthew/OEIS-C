/*
 * A381790  a(n) = 3*2^n + 2*n - 2.
 * https://oeis.org/A381790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n + 2*n - 2 (offset 0)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t power = pow2(n);          /* 2^n */
    seq_t result = 3 * power + (seq_t)n * 2 - 2;
    return result;
}
