/*
 * A250738  Number of (n+1) X (4+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nonincreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+1) + 30
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* a(0) is undefined for the OEIS offset */
    seq_t val = pow2(n + 1);         /* compute 2^(n+1) */
    val += (seq_t)30;                /* add the constant 30 */
    return val;
}
