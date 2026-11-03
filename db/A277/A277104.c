/*
 * A277104  a(n) = 9*3^n - 15.
 * https://oeis.org/A277104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*3^n - 15
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pow3 = ipow_idx((seq_t)3, n);
    seq_t result = (seq_t)9 * pow3 - (seq_t)15;
    return result;
}
