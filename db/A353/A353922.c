/*
 * A353922  Number of distinct pairs of partially ordered sets and legal
 *          permutations on labeled directed acyclic graphs (DAGs) with n
 *          nodes and up to n edges.
 * https://oeis.org/A353922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} binomial(binomial(n,2), k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 """
    if (n == 0) return (seq_t)1;

    /* Compute factorial n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* m = binomial(n,2) = n*(n-1)/2 */
    idx_t m = n * (n - 1U) / 2U;

    /* Compute partial sum S = Σ_{k=0}^{n} binomial(m,k) */
    seq_t sum = 0;
    seq_t bin = 1;          /* binomial(m,0) */
    sum += bin;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* binom(m,k) = binom(m,k-1) * (m-(k-1)) / k */
        bin = bin * (seq_t)(m - (k - 1U)) / (seq_t)k;
        sum += bin;
    }

    /* Return a(n) = n! * S */
    return fact * sum;
}
