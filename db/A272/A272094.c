/*
 * A272094  a(n) = Product_{k=0..n} binomial(k^2,k).
 * https://oeis.org/A272094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0..n} binomial(k^2,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t b = binomial(k * k, k);
        prod = seq_mul(prod, b);
    }
    return prod;
}
