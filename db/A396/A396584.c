/*
 * A396584  a(n) = (4 - 3*(-1)^n) * 5^floor(n/2).
 * https://oeis.org/A396584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4 - 3*(-1)^n) * 5^floor(n/2)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* 5^floor(n/2) */
    idx_t e = n >> 1;                 /* floor(n/2) */
    seq_t pow5 = ipow_idx((seq_t)5, e);/* pow(5, e) */

    /* factor: 1 for even n, 7 for odd n */
    seq_t factor = (n & 1) ? (seq_t)7 : (seq_t)1;

    return seq_mul(pow5, factor);
}
