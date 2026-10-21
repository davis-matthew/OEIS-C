/*
 * A250767  Number of (n+1) X (6+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 64*2^(n-1) + 84*n + 114; (n>=1)
 * Range:     u8 none, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* value outside the official offset */

    /* compute the closed‑form formula using the available pow2 helper */
    seq_t res = (seq_t)64 * pow2(n - 1) + (seq_t)84 * (seq_t)n + (seq_t)114;
    return res;
}
