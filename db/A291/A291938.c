/*
 * A291938  a(n) = 2^(n - 1) * (n - mod(n, 2)).
 * https://oeis.org/A291938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) * (n - mod(n, 2))
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* n is 1‑based according to OEIS; no action needed for n<1. */
    idx_t even_part = n & ~((idx_t)1);           /* n - (n mod 2) */
    seq_t power = pow2(n - (idx_t)1);            /* 2^(n-1) */
    seq_t result = power * (seq_t)even_part;     /* multiply by even part */
    return result;
}
