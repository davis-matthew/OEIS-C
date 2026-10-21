/*
 * A250765  Number of (n+1) X (4+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36*2^(n-1) + 10*n + 22 Empirical recurrence: a(n) =
 *            4*a(n-1) - 5*a(n-2) + 2*a(n-3) (n ≥ 4)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of offset 1 */
    seq_t term = pow2(n - 1);          /* 2^(n-1) as seq_t */
    term = term * 36 + (seq_t)(10 * n) + 22;
    return term;
}
