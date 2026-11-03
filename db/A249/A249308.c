/*
 * A249308  Central terms of triangle A249307.
 * https://oeis.org/A249308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * binomial(2*floor(n/2), floor(n/2))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 999 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* floor(n/2) */
    idx_t k = n >> 1;           /* unsigned shift is safe for idx_t */
    idx_t doublek = k + k;      /* 2*floor(n/2) */
    seq_t bin = binomial(doublek, k);   /* C(2k,k) */
    seq_t pwr = pow2(n);            /* 2^n  */
    return pwr * bin;
}
