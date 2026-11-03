/*
 * A304387  a(n) = 27*2^n - 5.
 * https://oeis.org/A304387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*2^n - 5
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 975 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pow2 = ipow_idx((seq_t)2, n);   /* 2^n */
    seq_t val  = (seq_t)27 * pow2 - (seq_t)5;
    return val;
}
