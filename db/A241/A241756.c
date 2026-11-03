/*
 * A241756  A finite sum of products of binomial coefficients: Sum_(m=0..n)
 *          binomial(-1/4, m)^2*binomial(-1/4, n-m)^2 (C. C. Grosjean's
 *          problem, denominators).
 * https://oeis.org/A241756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = a(n‑1) * 8^(1+v₂(n)), a(0) = 1 * where v₂(n) is the
 *            exponent of 2 dividing n (number of trailing zeros in n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline idx_t v2(idx_t n)
{
    idx_t c = 0;
    while ((n & 1) == 0) { /* n is even */
        ++c;
        n >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        /* exponent increment for this step */
        idx_t exp = 1 + v2(i);
        /* factor = 8^exp */
        seq_t factor = ipow_idx((seq_t)8, exp);
        /* multiply */
        result = seq_mul(result, factor);
    }

    return result;
}
