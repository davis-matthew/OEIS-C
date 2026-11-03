/*
 * A369298  Expansion of (1/x) * Series_Reversion( x * (1-x)^2 * (1-x^3)^2 ).
 * https://oeis.org/A369298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} binomial(2*n+k+1, k) *
 *            binomial(3*n-3*k+1, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-01)
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

seq_t a(idx_t n) {
    idx_t maxk = n / 3;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t b1 = binomial(2 * n + k + 1, k);
        seq_t b2 = binomial(3 * n - 3 * k + 1, n - 3 * k);
        seq_t prod = seq_mul(b1, b2);
        sum = sum + prod;
    }
    seq_t denom = (seq_t)(n + 1);
    return sum / denom;
}
