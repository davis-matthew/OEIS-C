/*
 * A334362  Number of r X s rectangles with integer sides such that r <= s, r
 *          + s = 2n and s/r is squarefree.
 * https://oeis.org/A334362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ i | 1 ≤ i ≤ n, 2n divisible by i, and μ(2n/i – 1) ≠ 0
 *            }
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mu_of.h"  /* provides int mu_of(idx_t d); */

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t two_n = ((seq_t)n) * 2;          /* cast to seq_t to avoid overflow */
    for (idx_t i = 1; i <= n; ++i)
    {
        if ((two_n % (seq_t)i) == 0)       /* only consider divisors of 2n */
        {
            idx_t t = (idx_t)((two_n / (seq_t)i) - 1);
            if (mu_of(t) != 0)             /* μ(t)≠0 ⇔ t is squarefree */
                ++total;
        }
    }
    return total;
}
