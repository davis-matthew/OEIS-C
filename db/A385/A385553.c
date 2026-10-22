/*
 * A385553  Period of {binomial(N,n) mod 6: N in Z}.
 * https://oeis.org/A385553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (smallest power of 2 > n) * (smallest power of 3 > n)
 * Range:     u8 n <= 8, u16 n <= 242, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the smallest power of 2 greater than n */
    seq_t p2 = 1;
    seq_t sn = (seq_t)n;
    while (p2 <= sn) {
        p2 <<= 1;
    }

    /* Find the smallest power of 3 greater than n */
    seq_t p3 = 1;
    while (p3 <= sn) {
        p3 *= 3;
    }

    return p2 * p3;
}
