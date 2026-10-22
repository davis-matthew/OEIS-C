/*
 * A385975  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(2*k,n-k).
 * https://oeis.org/A385975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(2*k,n-k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Compute the n-th term of OEIS sequence A385975. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term1 = binomial(n, k);
        seq_t term2 = binomial(2*k, n - k);
        sum += term1 * term2;
    }
    return sum;
}
