/*
 * A305268  a(n) = 82*2^n + 440.
 * https://oeis.org/A305268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 82*2^n + 440
 * Range:     u8 none, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 836 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two_pow = pow2(n);          /* 2^n */
    return (two_pow * (seq_t)82) + (seq_t)440;
}
