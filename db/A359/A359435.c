/*
 * A359435  a(n) = binomial(2*n-1,n) - n^2 - 1.
 * https://oeis.org/A359435
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-1, n) - n^2 - 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 27 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t two_n_minus1 = (n << 1) - 1;   /* 2*n - 1 */
    seq_t comb = binomial(two_n_minus1, n);
    seq_t n_sq = (seq_t)n * (seq_t)n;
    return comb - n_sq - 1;
}
