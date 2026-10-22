/*
 * A392640  Expansion of 1 / sqrt((1-x)^4 - 4*x^3).
 * https://oeis.org/A392640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*k,k) *
 *            binomial(n+k+1,n-3*k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n / 3;
    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t t1 = binomial(2 * k, k);
        seq_t t2 = binomial(n + k + 1, n - 3 * k);
        sum += t1 * t2;
    }
    return sum;
}
