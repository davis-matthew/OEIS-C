/*
 * A378461  a(n) = Sum_{k=0..n} binomial(n+k-1,k) * binomial(2*n+2*k-1,n-k).
 * https://oeis.org/A378461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k-1,k) * binomial(2*n+2*k-1,n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t c = binomial(n + k - 1, k);
        seq_t d = binomial(2 * n + 2 * k - 1, n - k);
        sum += c * d;
    }
    return sum;
}
