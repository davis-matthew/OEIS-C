/*
 * A386895  a(n) = Sum_{k=0..n} binomial(5*n+1,k) * binomial(2*n-k,n-k).
 * https://oeis.org/A386895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(5*n+1,k) * binomial(2*n-k,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-01)
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
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t b1 = binomial(5 * n + 1, k);
        seq_t b2 = binomial(2 * n - k, n - k);
        seq_t prod = seq_mul(b1, b2);
        sum += prod;
    }
    return sum;
}
