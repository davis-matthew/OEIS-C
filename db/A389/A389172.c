/*
 * A389172  a(n) = 2^(n+1) + 4^(n-1) - 1.
 * https://oeis.org/A389172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) + 4^(n-1) - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 486 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 in the OEIS offset, so n-1 is valid. */
    seq_t p2 = ipow_u64_idx(2, n + 1);
    seq_t p4 = ipow_u64_idx(4, n - 1);
    return p2 + p4 - 1;
}
