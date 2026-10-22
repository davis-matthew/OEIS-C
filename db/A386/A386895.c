/*
 * A386895  a(n) = Sum_{k=0..n} binomial(5*n+1,k) * binomial(2*n-k,n-k).
 * https://oeis.org/A386895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(5*n+1,k) * binomial(2*n-k,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t b1 = binomial(5 * n + 1, k);
        seq_t b2 = binomial(2 * n - k, n - k);
        seq_t prod = seq_mul(b1, b2);
        sum += prod;
    }
    return sum;
}
