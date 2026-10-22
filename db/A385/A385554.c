/*
 * A385554  Period of {binomial(N,n) mod 10: N in Z}.
 * https://oeis.org/A385554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (smallest power of 2 > n) * (smallest power of 5 > n)
 * Range:     u8 n <= 7, u16 n <= 124, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t pow2 = 1;         /* smallest power of 2 > n */
    while (pow2 <= (seq_t)n)
        pow2 <<= 1;

    seq_t pow5 = 1;         /* smallest power of 5 > n */
    while (pow5 <= (seq_t)n)
        pow5 *= (seq_t)5;

    return pow2 * pow5;
}
