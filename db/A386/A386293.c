/*
 * A386293  a(n) = 9^n - (1/2)*4^n, n > 0; a(0) = 0.
 * https://oeis.org/A386293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - (1/2)*4^n (n > 0), a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 297 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t pow9 = ipow_u64_idx(9, n);
    seq_t pow4 = ipow_u64_idx(4, n);

    seq_t half4 = pow4 / 2;
    return pow9 - half4;
}
