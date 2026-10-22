/*
 * A370098  a(n) = Sum_{k=0..n} binomial(3*n,k) * binomial(4*n-k-1,n-k).
 * https://oeis.org/A370098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*n,k) * binomial(4*n-k-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Computes the OEIS sequence A370098. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term1 = binom(3 * n, k);
        seq_t term2 = binom(4 * n - k - 1, n - k);
        sum += term1 * term2;
    }
    return sum;
}
