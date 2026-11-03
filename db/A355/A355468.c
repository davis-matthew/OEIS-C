/*
 * A355468  Expansion of Sum_{k>=0} (k^2 * x/(1 - k^2 * x))^k.
 * https://oeis.org/A355468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^(2*n) * binomial(n-1,k-1) for n>0; a(0)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 191 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
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
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    idx_t two_n = (idx_t)(2 * n);          /* exponent 2*n */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t bin = binomial(n - 1, k - 1);          /* C(n-1, k-1) */
        seq_t pow_k = ipow_idx((seq_t)k, two_n);      /* k^(2n) */
        seq_t prod = seq_mul(pow_k, bin);            /* product */
        sum += prod;                                 /* accumulate */
    }
    return sum;
}
