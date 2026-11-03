/*
 * A242985  a(n) = 4^n + 2^(n+1).
 * https://oeis.org/A242985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n + 2^(n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 97 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Compute 4^n */
    seq_t pow4n = ipow_u64_idx(4, n);
    /* Compute 2^(n+1) */
    seq_t pow2np1 = ipow_u64_idx(2, n + 1);
    return pow4n + pow2np1;
}
