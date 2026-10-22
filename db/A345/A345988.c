/*
 * A345988  Smallest oblong number m*(m+1) that is divisible by n.
 * https://oeis.org/A345988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m*(m+1) such that m*(m+1) is divisible by n
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 2, a(2) = 2 */
        return (seq_t)2;

    seq_t m = 1;
    for (;; ++m) {
        seq_t prod = m * (m + 1);      /* m*(m+1) */
        if (prod % (seq_t)n == 0)      /* divisible by n ? */
            return prod;
    }
}
