/*
 * A250741  Number of (n+1) X (7+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nonincreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+1) + 254 * (which satisfies the empirical recurrence
 *            a(n) = 3*a(n-1) - 2*a(n-2) with a(1) = 258)
 * Range:     u8 none, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n is at least 1, but we compute for any n >= 1. */
    return pow2(n + 1) + (seq_t)254;
}
