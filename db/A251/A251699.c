/*
 * A251699  a(n) = (7*n+1) * (8*n+1)^(n-2) * 9^n.
 * https://oeis.org/A251699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 229 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A251699: a(n) = (7*n+1) * (8*n+1)^(n-2) * 9^n   (offset 0) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Handle the first two indices explicitly (negative exponent otherwise). */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t term = (seq_t)(7 * n + 1);
    seq_t pow8   = ipow_idx((seq_t)(8 * n + 1), n - 2); /* (8*n+1)^(n-2) */
    seq_t pow9   = ipow_idx((seq_t)9, n);                /* 9^n */

    term = seq_mul(term, pow8);
    term = seq_mul(term, pow9);

    return term;
}
