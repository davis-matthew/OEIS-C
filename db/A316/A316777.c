/*
 * A316777  a(n) = Sum_{k=1..n} (k!)^5.
 * https://oeis.org/A316777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (k!)^5
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 114 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t fac = 1;
    for (idx_t k = 1; k <= n; ++k) {
        fac = seq_mul(fac, (seq_t)k);          /* fac = k!   */
        seq_t term = ipow_idx(fac, 5);          /* term = (k!)^5 */
        sum += term;                          /* accumulate */
    }
    return sum;
}
