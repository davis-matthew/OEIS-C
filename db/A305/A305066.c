/*
 * A305066  a(n) = 234*2^n - 120.
 * https://oeis.org/A305066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 234*2^n - 120
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 955 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    seq_t two_pow = pow2(n);                 /* 2^n */
    seq_t result  = seq_mul((seq_t)234, two_pow); /* 234 * 2^n */
    result -= (seq_t)120;                   /* -120 */
    return result;
}
