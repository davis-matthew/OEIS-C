/*
 * A377158  a(n) = Sum_{k=0..n} binomial(k+6,6) * binomial(k,n-k)^2.
 * https://oeis.org/A377158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+6,6) * binomial(k,n-k)^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 24, u128 n <= 24,
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
        seq_t c1 = binomial(k + 6, 6);
        seq_t c2 = binomial(k, n - k);
        seq_t term = c1 * c2;
        term *= c2;
        sum += term;
    }
    return sum;
}
