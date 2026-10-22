/*
 * A384374  Expansion of e.g.f. exp(x)*(exp(x) - 1)*(exp(x) - x - 1)^2.
 * https://oeis.org/A384374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - (2*n + 9)*3^(n-1) + (n^2 + 7*n + 12)*2^(n-2) - (n^2
 *            + n + 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* For n = 0 and n = 1 the formula involves negative exponents. */
    if (n < 2) return 0;

    /* Powers of 4, 3, and 2 */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);          /* 4^n */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 1));    /* 3^(n‑1) */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n - 2));    /* 2^(n‑2) */

    /* Construct the four parts of the formula */
    seq_t part1 = pow4;
    seq_t part2 = pow3 * ((seq_t)(2 * n + 9));
    seq_t expr3 = (seq_t)n * (seq_t)n + (seq_t)7 * (seq_t)n + 12;
    seq_t part3 = pow2 * expr3;
    seq_t part4 = (seq_t)n * (seq_t)n + n + 1;

    /* Compute a(n) as (part1 + part3) - (part2 + part4) */
    seq_t sum1 = part1 + part3;
    seq_t sum2 = part2 + part4;
    return sum1 - sum2;
}
