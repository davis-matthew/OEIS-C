/*
 * A275163  a(n) = 13*2^(n+1) - 19.
 * https://oeis.org/A275163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*2^(n+1) - 19
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n + 1);      /* 2^(n+1) */
    val = val * (seq_t)13;        /* 13 * 2^(n+1) */
    val = val - (seq_t)19;        /* minus 19 */
    return val;
}
