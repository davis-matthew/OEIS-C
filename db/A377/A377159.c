/*
 * A377159  a(n) = Sum_{k=0..n} binomial(k+7,7) * binomial(k,n-k)^2.
 * https://oeis.org/A377159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+7,7) * binomial(k,n-k)^2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin1 = binomial(k + 7, 7);
        seq_t bin2 = binomial(k, n - k);
        /* Compute bin1 * bin2 * bin2 using seq_t arithmetic */
        seq_t prod = bin1 * bin2;
        prod = prod * bin2;
        sum += prod;
    }
    return sum;
}
