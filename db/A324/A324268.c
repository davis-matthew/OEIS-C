/*
 * A324268  a(n) = 31*11^(5*n).
 * https://oeis.org/A324268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*161051^n
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* 161051 = 11^5 */
    seq_t base = (seq_t)161051;
    seq_t exp  = (seq_t)n;
    seq_t pow  = ipow(base, exp);
    return seq_mul(pow, (seq_t)31);
}
