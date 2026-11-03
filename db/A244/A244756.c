/*
 * A244756  a(n) = Sum_{k=0..n} C(n,k) * (2 + 3^k)^(n-k).
 * https://oeis.org/A244756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * (2 + 3^k)^(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 78 (2026-11-01)
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

/* Computes the n-th term of OEIS sequence A244756.
   Works for any n >= 0. */
seq_t a(idx_t n)
{
    /* Accumulator for the sum */
    seq_t sum = 0;
    /* 3^k, starting with k = 0 */
    seq_t pow3k = 1;

    for (idx_t k = 0; k <= n; ++k) {
        /* Binomial coefficient C(n,k) */
        seq_t bin = binomial(n, k);

        /* Base = 2 + 3^k */
        seq_t base = pow3k + 2;

        /* Power = base^(n-k) */
        seq_t power = ipow_idx(base, n - k);

        /* term = C(n,k) * power */
        seq_t term = seq_mul(bin, power);

        /* Add to the running sum */
        sum += term;

        /* Update pow3k for next iteration (unless last) */
        if (k < n)
            pow3k = seq_mul(pow3k, 3);
    }

    return sum;
}
