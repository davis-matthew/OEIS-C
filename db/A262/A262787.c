/*
 * A262787  a(n) = 41^(2*n+1).
 * https://oeis.org/A262787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 41^(2*n+1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 249 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    idx_t exp = 2 * n + 1;
    return ipow_idx((seq_t)41, exp);
}
