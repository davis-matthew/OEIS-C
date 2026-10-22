/*
 * A397384  a(n) = Sum_{k=0..n} binomial(n+2*k,3*k) * binomial(n+3*k,k-1).
 * https://oeis.org/A397384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+2*k,3*k) * binomial(n+3*k,k-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;

    /* k = 0 term is zero because binomial(…, -1) = 0 */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term1 = binomial(n + 2 * k, 3 * k);
        seq_t term2 = binomial(n + 3 * k, k - 1);
        sum += term1 * term2;
    }

    return sum;
}
