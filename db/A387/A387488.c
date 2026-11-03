/*
 * A387488  a(n) = [x^n] hypergeom([1/2, 1/2], [1], 256*x).
 * https://oeis.org/A387488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16^n * C(2n,n)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
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
    /* Compute the central binomial coefficient C(2n,n) */
    seq_t binom = 1;
    for (idx_t k = 1; k <= n; ++k) {
        binom = seq_mul(binom, (seq_t)(n + k));   /* multiply by (n+k) */
        binom = binom / (seq_t)k;                /* exact division by k */
    }

    /* Square the binomial coefficient */
    seq_t binom_sq = seq_mul(binom, binom);

    /* Compute 16^n */
    seq_t pow16 = ipow_idx((seq_t)16, n);

    /* a(n) = 16^n * binom_sq */
    return seq_mul(pow16, binom_sq);
}
