/*
 * A383641  a(n) is the difference between the sum of even composites and the
 *          sum of the odd composites in the first n positive integers.
 * https://oeis.org/A383641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum of even composites ≤ n) – (sum of odd composites ≤
 *            n) Computation: iterate 2..n, test if composite by trial
 *            division, accumulate even or odd composite sums, return their
 *            difference
 * Range:     u8 n <= 41, u16 n <= 919, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* no composites below 2 */

    seq_t evenComp = 0;
    seq_t oddComp  = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* test primality by trial division up to sqrt(i) */
        bool prime = true;
        if (i > 2) {
            for (idx_t p = 2; p * p <= i; ++p) {
                if (i % p == 0) { prime = false; break; }
            }
        }
        /* i is composite if not prime and >1 */
        if (!prime) {
            if ((i & 1) == 0)
                evenComp += (seq_t)i;
            else
                oddComp += (seq_t)i;
        }
    }
    return evenComp - oddComp;
}
