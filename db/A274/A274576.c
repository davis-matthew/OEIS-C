/*
 * A274576  a(2n) = floor(n/2), a(2n+1) = a(n), a(0)=0.
 * https://oeis.org/A274576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = floor(n/2); a(2n+1) = a(n); a(0) = 0
 * Range:     u8 n <= 109, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Collapse odd indices by repeated halving; when even we return n/4 */
    while ((n & 1U) == 1U)
        n >>= 1;           /* n = floor(n/2) for odd n */
    return (seq_t)(n >> 2); /* n/4, integer division, works for all n (including 0) */
}
