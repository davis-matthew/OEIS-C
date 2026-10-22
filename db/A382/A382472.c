/*
 * A382472  a(n) = Sum_{k=0..n} binomial(k+5,5) * binomial(2*k,2*n-2*k).
 * https://oeis.org/A382472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+5,5) * binomial(2*k,2*n-2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 34, u128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t c1 = binomial(k + 5, 5);          /* C(k+5,5) */
        seq_t c2 = binomial(2 * k, 2 * n - 2 * k); /* C(2k,2n-2k) */
        sum += c1 * c2;
    }
    return sum;
}
