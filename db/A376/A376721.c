/*
 * A376721  Expansion of 1/sqrt((1 - x^3 - x^4)^2 - 4*x^7).
 * https://oeis.org/A376721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(k,n-3*k)^2
 * Range:     u8 n <= 20, u16 n <= 34, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t max_k = n / 3;
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t r = n - 3 * k;
        if (r <= k) {
            seq_t c = binomial(k, r);
            sum += c * c;
        }
    }
    return sum;
}
