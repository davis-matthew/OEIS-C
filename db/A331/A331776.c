/*
 * A331776  Number of regions in a "frame" of size n X n (see Comments for
 *          definition).
 * https://oeis.org/A331776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*n*(n-1) + 4*Sum_{i=2..n} (n+1-i)*(2n+2-i)*phi(i) (n>1)
 *            a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 288, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 1u)
        return (seq_t)4u;

    seq_t sum = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term1 = (seq_t)(n + 1 - i);
        seq_t term2 = (seq_t)(2 * n + 2 - i);
        seq_t phi_val = phi(i);                  /* static phi from utils/phi.h */
        seq_t term = term1 * term2;
        term *= phi_val;
        sum += term;
    }

    seq_t n_sq = (seq_t)n * (seq_t)(n - 1);
    seq_t res = (seq_t)20 * n_sq + (seq_t)4 * sum;
    return res;
}
