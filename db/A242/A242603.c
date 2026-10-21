/*
 * A242603  Largest divisor of n not divisible by 7. Remove factors 7 from n.
 * https://oeis.org/A242603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n / 7^k where 7^k is the highest power of 7 dividing n
 * Range:     u8 n <= 255, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Remove all factors of 7 from n */
    while (n % 7 == 0) {
        n /= 7;
    }
    return (seq_t)n;
}
