/*
 * A302489  Total domination number of the n-cube connected cycle graph.
 * https://oeis.org/A302489
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 6 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for A302489:  a(n) = 2^(n-3) * (2n + 4)  for n >= 4,
   and the special value a(3) = 8. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_u64_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 3) {
        return (seq_t)8;
    }
    seq_t pow_two = ipow_u64_idx(2, n - 3);          /* 2^(n-3) */
    seq_t factor = (seq_t)(2 * n + 4);           /* (2n + 4)   */
    return seq_mul(pow_two, factor);            /* product     */
}
