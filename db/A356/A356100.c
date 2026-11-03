/*
 * A356100  a(n) = Sum_{k=1..n} (k - 1)^n * floor(n/k).
 * https://oeis.org/A356100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (k - 1)^n * floor(n/k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t base = (seq_t)(k - 1);
        seq_t power = ipow_idx(base, n);           /* (k-1)^n   */
        seq_t mult = (seq_t)(n / k);              /* floor(n/k) */
        result += power * mult;
    }
    return result;
}
