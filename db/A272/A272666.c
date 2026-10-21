/*
 * A272666  a(n) = A011371(n) + 5*n.
 * https://oeis.org/A272666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 6*n - popcount(n)
 * Range:     u8 n <= 43, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n * 6;                /* 6*n in the correct type */

    /* Count the number of set bits in n (popcount) */
    idx_t x = n;
    unsigned pop = 0;
    while (x) {
        pop++;
        x &= x - 1;                         /* clear the lowest set bit */
    }

    val -= (seq_t)pop;                      /* subtract popcount(n)   */
    return val;
}
