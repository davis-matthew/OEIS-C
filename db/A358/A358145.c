/*
 * A358145  a(n) = Sum_{k=0..n} binomial(n*k,k) * binomial(n*(n-k),n-k).
 * https://oeis.org/A358145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n*k, k) * binomial(n*(n-k), n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term1 = binomial((idx_t)n * k, k);
        seq_t term2 = binomial(n * (n - k), n - k);
        sum += term1 * term2;
    }
    return sum;
}
