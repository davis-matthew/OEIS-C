/*
 * A305166  a(n) = 164*2^n - 140.
 * https://oeis.org/A305166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 164*2^n - 140
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);   /* 2^n */
    val *= 164;            /* 164 * 2^n */
    val -= 140;            /* subtract 140 */
    return val;
}
