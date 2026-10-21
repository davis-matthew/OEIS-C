/*
 * A250655  Number of (n+1) X (7+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 44*2^n - 7 (≡ a(n)=3*a(n-1)-2*a(n-2), a(1)=81, a(2)=169)
 *            The sequence grows as a linear combination of 1^n and 2^n
 *            weights. The formula is computed via a fast exponentiation
 *            routine from the utils
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence. The formula works for n = 1 as well. */
    seq_t t = pow_uint((uint64_t)2, n);  /* compute 2^n */
    t = t * (seq_t)44;                   /* multiply by 44 */
    t = t - (seq_t)7;                    /* subtract 7 */
    return t;
}
