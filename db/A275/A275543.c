/*
 * A275543  A081585 and A069129 interleaved.
 * https://oeis.org/A275543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + (-1)^n
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 9762, u64 n <= 9762,
 *            u128 n <= 9762, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t res = 2 * n_t * n_t;
    if ((n & 1) != 0)
        res -= 1;   /* n is odd : subtract 1 */
    else
        res += 1;   /* n is even: add 1 */
    return res;
}
