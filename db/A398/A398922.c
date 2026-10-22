/*
 * A398922  Number of distinct acute lattice trapezia of area n/2 with
 *          horizontal base.
 * https://oeis.org/A398922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   f_of_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/f_of_m.h"
/*  OEIS A398922
 *  Number of distinct acute lattice trapezia of area n/2 with horizontal base.
 *  Equivalent combinatorial formula:
 *     a(n) = Σ_{d|n} f(n/d)
 *  where
 *     f(m) = 0                     if m < 4
 *     f(m) = (t+1) * (K+1 - t)     otherwise,
 *     with K = m-4   and   t = floor(K/2).
 *  The function below implements this formula.  It enumerates all divisors
 *  of n in O(√n) time, which is well below the required limits for n≤10000.
 */

seq_t a(idx_t n)
{
    /* Iterate over all divisors d of n.  For each divisor we compute
       m = n/d and add f(m) to the sum. */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t m1 = n / d;            /* divisor pair (d, n/d) */
            sum += f_of_m(m1);
            if (d * d != n) {            /* add the complementary divisor */
                seq_t m2 = d;
                sum += f_of_m(m2);
            }
        }
    }
    return sum;
}
