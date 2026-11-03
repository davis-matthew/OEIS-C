/*
 * A350869  a(n) = Sum_{i=0..10^n-1} i^3.
 * https://oeis.org/A350869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^(2*n) * (10^n - 1)^2 / 4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n) {
    /* 10^n */
    seq_t t = ipow_u64_idx(10, n);
    /* 10^(2n) */
    seq_t d = ipow_u64_idx(10, 2 * n);
    /* (10^n - 1)^2 */
    seq_t u = t - 1;
    seq_t v = u * u;
    /* 10^(2n) * (10^n - 1)^2 / 4 */
    seq_t prod = d * v;
    return prod / 4;
}
