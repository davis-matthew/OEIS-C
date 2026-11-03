/*
 * A245391  a(n) = 2^n*binomial(2*(n+1), n).
 * https://oeis.org/A245391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * binomial(2*(n+1), n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 990 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(2*(n+1), n) */
    seq_t bin_val = binomial(2 * (n + 1), n);

    /* Compute 2^n */
    seq_t two_pow = pow2(n);

    /* Return the product */
    return seq_mul(two_pow, bin_val);
}
