/*
 * A370272  Coefficient of x^n in the expansion of 1/( (1-x) * (1-x^3) )^n.
 * https://oeis.org/A370272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n+k-1,k) *
 *            binomial(2*n-3*k-1,n-3*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t sum = 0;
    idx_t kmax = n / 3;
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t b1 = binom(n + k - 1, k);
        seq_t b2 = binom(2 * n - 3 * k - 1, n - 3 * k);
        sum += b1 * b2;
    }
    return sum;
}
