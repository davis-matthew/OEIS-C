/*
 * A251696  a(n) = (4*n+1) * (5*n+1)^(n-2) * 6^n.
 * https://oeis.org/A251696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n+1) * (5*n+1)^(n-2) * 6^n (for n >= 2); special
 *            cases: a(0) = 1, a(1) = 5
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 248 (2026-11-01)
 * Helpers:   ipow, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* 6^n */
    seq_t p6 = ipow_idx((seq_t)6, n);

    /* (5*n+1)^(n-2) */
    seq_t base = (seq_t)(5 * n + 1);
    seq_t exp  = (seq_t)(n - 2);
    seq_t pow5 = ipow(base, exp);

    /* (4*n+1) * pow5 */
    seq_t tem = seq_mul((seq_t)(4 * n + 1), pow5);

    /* final result */
    return seq_mul(tem, p6);
}
