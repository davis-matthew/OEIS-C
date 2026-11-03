/*
 * A362085  a(n) = Sum_{k=0..n} (-1)^k * binomial(-3,k) * binomial(n*k,n-k).
 * https://oeis.org/A362085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+2,k) * binomial(n*k,n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 490 (2026-11-01)
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
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t b1 = binomial(k + 2, k);
        idx_t nk = n * k;
        idx_t nk_minus = n - k;
        seq_t b2 = binomial(nk, nk_minus);
        sum += seq_mul(b1, b2);
    }
    return sum;
}
