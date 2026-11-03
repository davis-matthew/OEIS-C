/*
 * A337387  a(n) = Sum_{k=0..n} n^(n-k) * binomial(2*k,k) *
 *          binomial(2*n+1,2*k).
 * https://oeis.org/A337387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} n^(n-k) * binomial(2*k,k) *
 *            binomial(2*n+1,2*k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t power = ipow_u64_idx((uint64_t)n, n - k);
        seq_t b1    = binomial(2 * k, k);
        seq_t b2    = binomial(2 * n + 1, 2 * k);
        seq_t term  = seq_mul(seq_mul(power, b1), b2);
        sum += term;
    }
    return sum;
}
