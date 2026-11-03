/*
 * A283369  a(n) = Sum_{d|n} d^(4*d + 1).
 * https://oeis.org/A283369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(4*d + 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 118 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor i */
            seq_t term1 = ipow_u64_idx((uint64_t)i, (idx_t)(4 * i + 1));
            sum += term1;
            /* divisor n/i, avoid double‑count when i == n/i */
            if (i != n / i) {
                idx_t d2 = n / i;
                seq_t term2 = ipow_u64_idx((uint64_t)d2, (idx_t)(4 * d2 + 1));
                sum += term2;
            }
        }
    }
    return sum;
}
