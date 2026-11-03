/*
 * A350128  a(n) = Sum_{k=1..n} k^n * floor(n/k)^2.
 * https://oeis.org/A350128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^n * floor(n/k)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 360 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    /* a(1) is the first term of the sequence */
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t power = ipow_idx((seq_t)k, n);
        idx_t t = n / k;
        seq_t t_sq = (seq_t)t * (seq_t)t;
        result += power * t_sq;
    }
    return result;
}
