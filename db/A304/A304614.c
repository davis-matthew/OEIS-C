/*
 * A304614  a(n) = 420*2^n - 222.
 * https://oeis.org/A304614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 420*2^n - 222
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t t = pow2(n);       /* 2^n in arbitrary precision */
    return 420 * t - 222;    /* 420*2^n - 222 */
}
