/*
 * A321295  a(n) = n * sigma_n(n).
 * https://oeis.org/A321295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * sigma_n(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 371 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            /* d^n */
            seq_t power = ipow((seq_t)d, (seq_t)n);
            sum += power;
        }
    }
    return sum * (seq_t)n;
}
