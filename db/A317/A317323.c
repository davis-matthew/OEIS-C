/*
 * A317323  Multiples of 23 and odd numbers interleaved.
 * https://oeis.org/A317323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(2n) = 23*n, a(2n+1) = 2*n+1
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t k = n >> 1; /* n/2 */
    if ((n & 1ULL) == 0) { /* even index */
        return (seq_t)23 * (seq_t)k;
    } else { /* odd index */
        return (seq_t)2 * (seq_t)k + (seq_t)1;
    }
}
