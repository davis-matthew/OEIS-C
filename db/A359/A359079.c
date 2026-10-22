/*
 * A359079  a(n) is the sum of the divisors d of 2*n such that the binary
 *          expansions of d and 2*n have no common 1-bit.
 * https://oeis.org/A359079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{d|2n, d & 2n == 0} d
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute m = 2n.  n <= 10000, so m fits into idx_t safely. */
    idx_t m = n * 2;

    /* Compute integer square root of m by simple incremental search. */
    idx_t sqrt_m = 0;
    while ((sqrt_m + 1) * (sqrt_m + 1) <= m) {
        sqrt_m++;
    }

    seq_t sum = 0;
    for (idx_t d = 1; d <= sqrt_m; ++d) {
        if (m % d == 0) {
            seq_t sd = (seq_t)d;
            if ((sd & (seq_t)m) == 0) {
                sum += sd;
            }
            idx_t e = m / d;
            if (e != d) {
                seq_t se = (seq_t)e;
                if ((se & (seq_t)m) == 0) {
                    sum += se;
                }
            }
        }
    }
    return sum;
}
