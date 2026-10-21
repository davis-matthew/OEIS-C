/*
 * A272342  a(n) = 27*8^n.
 * https://oeis.org/A272342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*8^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t exp = n * 3;                      /* 8^n = 2^(3n) */
    return pow2(exp) * (seq_t)27;           /* multiply by 27 */
}
