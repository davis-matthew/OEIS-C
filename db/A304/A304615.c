/*
 * A304615  a(n) = 507*2^n - 273.
 * https://oeis.org/A304615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 507*2^n - 273
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow = pow2(n);
    seq_t result = 507 * pow;
    result -= 273;
    return result;
}
