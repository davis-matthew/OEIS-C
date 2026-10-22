/*
 * A382642  a(n) = Sum_{k=0..n} (binomial(n,k) * binomial(n+k,k))^2 * 2^(n-k).
 * https://oeis.org/A382642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (C(n,k)*C(n+k,k))^2 * 2^(n-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t b1 = binomial(n, k);
        seq_t b2 = binomial(n + k, k);
        seq_t prod = b1 * b2;
        prod = prod * prod;
        prod = prod * pow2(n - k);
        sum += prod;
    }

    return sum;
}
