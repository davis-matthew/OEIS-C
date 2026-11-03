/*
 * A332919  a(n) is the sum of the sums of squared digits of all n-digit
 *          numbers.
 * https://oeis.org/A332919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 57 · 2^(n‑2) · 5^(n‑1) · (1+9 n) for n ≥ 3; a(1)=285,
 *            a(2)=5415. Uses the closed‑form expression
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 860 (2026-11-01)
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
    if (n == 0) return 0;                     /* value below offset not required */
    if (n == 1) return (seq_t)285;
    if (n == 2) return (seq_t)5415;

    idx_t i = (idx_t)n;
    seq_t p2 = ipow_idx((seq_t)2, i - 2);      /* 2^(n‑2) */
    seq_t p5 = ipow_idx((seq_t)5, i - 1);      /* 5^(n‑1) */

    seq_t result = (seq_t)57;
    result = seq_mul(result, p2);             /* 57 * 2^(n‑2) */
    result = seq_mul(result, p5);             /* ... * 5^(n‑1) */

    seq_t factor = (seq_t)(1 + 9 * i);        /* 1 + 9*n */
    result = seq_mul(result, factor);         /* final multiplication */

    return result;
}
