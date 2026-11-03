/*
 * A336873  a(n) = Sum_{k=0..n} (binomial(n+k,k) * binomial(n,k))^n.
 * https://oeis.org/A336873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (binomial(n+k,k) * binomial(n,k))^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 36 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t c1 = binomial(n + k, k);
        seq_t c2 = binomial(n, k);
        seq_t prod = seq_mul(c1, c2);
        seq_t pow_term = ipow_idx(prod, n);
        sum = sum + pow_term;
    }
    return sum;
}
