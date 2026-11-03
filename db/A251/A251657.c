/*
 * A251657  a(n) = (2^n + 3)^n.
 * https://oeis.org/A251657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n + 3)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-01)
 * Helpers:   ipow_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t base = pow2(n) + (seq_t)3;
    return ipow_idx(base, n);
}
