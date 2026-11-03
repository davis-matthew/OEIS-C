/*
 * A385086  a(n) = 2 * (5*n+2)^(n-1).
 * https://oeis.org/A385086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (5*n + 2)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
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
    if (n == 0)
        return (seq_t)1;                 /* a(0) = 1 */

    seq_t base = (seq_t)n * (seq_t)5 + (seq_t)2; /* 5*n + 2 */
    seq_t pow_val = ipow_idx(base, n - 1);        /* (5*n+2)^(n-1) */
    return seq_mul(pow_val, (seq_t)2);           /* 2 * base^(n-1) */
}
