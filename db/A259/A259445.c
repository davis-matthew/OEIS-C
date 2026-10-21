/*
 * A259445  Multiplicative with a(n) = n if n is odd and a(2^s)=2.
 * https://oeis.org/A259445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n if n is odd; otherwise a(n) = 2 * (odd part of n) where
 *            odd part = n / 2^v2(n) and v2(n) is the exponent of 2 in n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is at least 1 */
    if ((n & 1) == 0) {
        /* Find the odd part by shifting right until an odd value remains. */
        idx_t odd = n;
        while ((odd & 1) == 0) odd >>= 1;
        /* Multiply the odd part by 2.  This value is <= n and fits in seq_t. */
        return (seq_t)(odd * 2);
    } else {
        /* For odd n, a(n) = n. */
        return (seq_t)n;
    }
}
