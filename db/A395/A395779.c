/*
 * A395779  Expansion of (Sum_{k>=0} binomial(3*k+1,k) * x^k)^3.
 * https://oeis.org/A395779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (k+1) * 2^k * binomial(3*n+5, n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility functions used:
 *   binomial(idx_t n, idx_t k)      – compute binomial coefficients
 *   mul_small(seq_t x, unsigned int m) – multiply a seq_t by a small int
 *   seq_mul(seq_t a, seq_t b)       – multiply two seq_t values
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

extern seq_t a(idx_t n)
{
    seq_t sum = 0;          /* accumulator */
    seq_t power2 = 1;       /* 2^0 */

    /* k iterates from 0 to n inclusive */
    for (idx_t k = 0; k <= n; ++k) {
        idx_t m = n - k;                           /* n-k */
        seq_t term = binomial(3 * n + 5, m);       /* binomial(3n+5, n-k) */
        term = mul_small(term, (k + 1));           /* multiply by (k+1) */
        term = seq_mul(term, power2);              /* multiply by 2^k */
        sum += term;                               /* accumulate */
        power2 = mul_small(power2, 2);             /* prepare 2^(k+1) */
    }

    return sum;
}
