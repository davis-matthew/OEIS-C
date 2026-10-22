/*
 * A383456  a(n) = 2*binomial(2*n+4,n)*(n+3)/(n+2).
 * https://oeis.org/A383456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*binomial(2*n+4,n)*(n+3)/(n+2)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Compute the binomial coefficient C(2*n+4, n) */
    idx_t m = 2*n + 4;
    seq_t bin = binomial(m, n);

    /* a(n) = 2 * bin * (n+3) / (n+2) */
    seq_t result = bin * (seq_t)2;
    result = result * (seq_t)(n + 3);
    result = result / (seq_t)(n + 2);

    return result;
}
