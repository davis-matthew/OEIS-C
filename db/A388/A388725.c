/*
 * A388725  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(2*n+3*k,k).
 * https://oeis.org/A388725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(2*n+3*k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t c1 = binomial(n, k);
        seq_t c2 = binomial(2 * n + 3 * k, k);
        sum += c1 * c2;
    }
    return sum;
}
