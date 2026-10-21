/*
 * A250739  Number of (n+1) X (5+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nonincreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+1) + 62
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For safety we return 0 for
       indexes below the offset, but the function is never called with
       such values in the test harness. */
    if (n < 1) return 0;
    seq_t val = pow2(n + 1);
    return val + (seq_t)62;
}
