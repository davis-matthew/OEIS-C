/*
 * A275827  a(n) = Sum_{k=0..n} binomial(n+k+2,k)*binomial(2*n+1,n-k).
 * https://oeis.org/A275827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k+2, k) * binomial(2*n+1, n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"


seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term1 = binomial(n + k + 2, k);
        seq_t term2 = binomial(2 * n + 1, n - k);
        sum += term1 * term2;
    }
    return sum;
}
