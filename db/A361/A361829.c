/*
 * A361829  a(n) = Sum_{k=0..n} binomial(2*k,k) * binomial(n*k,n-k).
 * https://oeis.org/A361829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*k, k) * binomial(n*k, n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
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
        seq_t c1 = binomial(2 * k, k);        /* binomial(2k, k) */
        seq_t c2 = binomial(n * k, n - k);    /* binomial(nk, n-k) */
        sum += c1 * c2;
    }
    return sum;
}
