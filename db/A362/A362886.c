/*
 * A362886  Antidiagonal sums of A362885.
 * https://oeis.org/A362886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (1 + 2*(n - k))*k^(n-k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t coeff = 1 + (seq_t)2 * (seq_t)(n - k);
        seq_t powk  = big_pow((seq_t)k, (seq_t)(n - k));
        sum += coeff * powk;
    }
    return sum;
}
