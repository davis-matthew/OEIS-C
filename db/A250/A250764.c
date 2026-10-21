/*
 * A250764  Number of (n+1) X (3+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*2^(n-1) + 2*n + 8 (for n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* safety for n == 0, though not used */
    seq_t res = (seq_t)25 * pow2(n - 1);  /* 25 * 2^(n-1) */
    res += (seq_t)2 * (seq_t)n;          /* + 2*n */
    res += (seq_t)8;                     /* + 8 */
    return res;
}
