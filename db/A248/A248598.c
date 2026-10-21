/*
 * A248598  a(n) = (2*n+23)*n*(n-1), a coefficient appearing in the formula
 *          a(n)*Pi/324+n+1 giving the average number of regions into which n
 *          random planes divide the cube.
 * https://oeis.org/A248598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+23)*n*(n-1)
 * Range:     u8 n <= 3, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t nn = (seq_t)n;
    /* compute (2*nn + 23) * nn * (nn - 1) */
    return ((seq_t)(2 * nn + 23) * nn * (nn - 1));
}
