/*
 * A302353  a(n) = Sum_{k=0..n} k^n*binomial(2*n-k,n).
 * https://oeis.org/A302353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^n*binomial(2*n-k,n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t power = big_pow((seq_t)k, (seq_t)n);
        seq_t coeff = binomial(2 * n - k, n);
        sum += power * coeff;
    }
    return sum;
}
