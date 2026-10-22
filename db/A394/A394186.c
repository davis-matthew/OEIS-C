/*
 * A394186  a(n) = Sum_{k=0..n} binomial(n+k,2*k) * (binomial(n+2*k,k) -
 *          binomial(n+2*k,k-1)).
 * https://oeis.org/A394186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k,2*k) * (binomial(n+2*k,k) -
 *            binomial(n+2*k,k-1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
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
        seq_t b1 = binomial(n + k, 2 * k);
        seq_t diff = binomial(n + 2 * k, k);
        if (k) diff -= binomial(n + 2 * k, k - 1);
        sum += b1 * diff;
    }
    return sum;
}
