/*
 * A295286  Sum of the products of the smaller and larger parts of the
 *          partitions of n into two parts with the smaller part odd.
 * https://oeis.org/A295286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} i*(n-i)*(i mod 2)
 * Range:     u8 n <= 17, u16 n <= 116, u32 n <= 4689, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    idx_t limit = n >> 1;          /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t i = 1; i <= limit; ++i) {
        if ((i & 1) != 0) {        /* i is odd */
            seq_t ii  = (seq_t)i;
            seq_t term = ii * ((seq_t)n - ii);
            sum += term;
        }
    }

    return sum;
}
