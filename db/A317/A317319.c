/*
 * A317319  Multiples of 19 and odd numbers interleaved.
 * https://oeis.org/A317319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(2n) = 19*n , a(2n+1) = 2*n+1
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Divide n by 2 using a right shift (idx_t is unsigned) */
    seq_t half = (seq_t)(n >> 1);

    /* Even indices */
    if ((n & 1) == 0)
        return half * (seq_t)19;

    /* Odd indices */
    return ((seq_t)2 * half) + (seq_t)1;
}
