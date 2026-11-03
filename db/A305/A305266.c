/*
 * A305266  a(n) = 14*2^n + 73.
 * https://oeis.org/A305266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 73 + 14 * 2^n
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 28, u64 n <= 31, u128 n <= 31,
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
    seq_t p = pow2(n);                 /* 2^n  */
    return p * (seq_t)14 + (seq_t)73;  /* 14*2^n + 73 */
}
