/*
 * A376792  Expansion of 1/sqrt((1 - x^4)^2 - 4*x).
 * https://oeis.org/A376792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(2*n-7*k, k) *
 *            binomial(2*n-8*k, n-4*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t maxk = n / 4;
    seq_t result = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term1 = binomial(2*n - 7*k, k);
        seq_t term2 = binomial(2*n - 8*k, n - 4*k);
        result += term1 * term2;
    }
    return result;
}
