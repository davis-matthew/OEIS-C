/*
 * A350668  Numbers congruent to 2, 4, and 6 modulo 9: positions of 2 in
 *          A159955.
 * https://oeis.org/A350668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = 2 + 9k a(3k+1) = 4 + 9k a(3k+2) = 6 + 9k for k >= 0
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t q = n / 3;          /* floor(n/3) as idx_t */
    idx_t r = n % 3;          /* remainder 0,1,2 */

    seq_t base;
    if (r == 0)
        base = 2;
    else if (r == 1)
        base = 4;
    else
        base = 6;

    return base + (seq_t)9 * (seq_t)q;
}
