/*
 * A291778  a(n) = 2^floor(2*n/3).
 * https://oeis.org/A291778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^floor(2*n/3)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 95, u128 n <= 191,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t q = n / 3;
    idx_t r = n % 3;
    idx_t e = q * 2 + (r == 2 ? 1 : 0);
    return pow2(e);
}
