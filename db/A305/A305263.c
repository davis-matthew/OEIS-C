/*
 * A305263  a(n) = 680*2^n - 622.
 * https://oeis.org/A305263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 680*2^n - 622
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 22, u64 n <= 54, u128 n <= 118,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 680 * 2^n - 622, using the provided pow2 utility */
    seq_t power = pow2(n);
    seq_t result = (seq_t)680 * power;
    result -= (seq_t)622;
    return result;
}
