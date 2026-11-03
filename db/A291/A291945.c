/*
 * A291945  Powers of 1111.
 * https://oeis.org/A291945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1111^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* 1111 is within seq_t for all build configurations,
       so it can be safely cast to seq_t. */
    return ipow_idx((seq_t)1111, n);
}
