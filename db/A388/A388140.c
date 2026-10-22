/*
 * A388140  Number of axis-aligned 2 X 2 squares in the 3D projection of a
 *          3n-dimensional hypercube with side length 2.
 * https://oeis.org/A388140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n*(2^n-1)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.
       Return 0 for n == 0 to handle calls below the offset. */
    if (n == 0) return 0;

    seq_t two_n = pow2(n);          /* 2^n   */
    seq_t tmp   = two_n - 1;        /* 2^n-1 */

    /* Compute 3 * two_n * tmp * tmp */
    seq_t result = 3;
    result *= two_n;
    result *= tmp;
    result *= tmp;

    return result;
}
