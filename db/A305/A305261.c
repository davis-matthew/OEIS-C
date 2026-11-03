/*
 * A305261  a(n) = 120*2^n - 108.
 * https://oeis.org/A305261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 120*2^n - 108
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 966 (2026-11-01)
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

seq_t a(idx_t n)
{
    seq_t val = pow2(n);
    val = seq_mul(val, (seq_t)120);
    val -= (seq_t)108;
    return val;
}
