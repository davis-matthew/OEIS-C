/*
 * A356259  Number of labeled rooted trees on [n] that have a primary branch.
 * https://oeis.org/A356259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} binomial(n,i) * (n-i)^(n-i-1) *
 *            i^(i-1), with a(1)=0. Here r(k)=k^(k-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t total = 0;
    idx_t half = n / 2;
    for (idx_t i = 1; i <= half; ++i)
    {
        seq_t bin = binomial(n, i);
        seq_t pow1 = big_pow((seq_t)(n - i), (seq_t)(n - i - 1));
        seq_t pow2 = big_pow((seq_t)i, (seq_t)(i - 1));
        seq_t prod = bin * pow1;
        prod = prod * pow2;
        total += prod;
    }
    return total;
}
