/*
 * A250773  Number of (5+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*2^(n-1) + 11*n^2 + 57*n + 52
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^(n-1) */
    seq_t two_exp = pow2(n - 1);

    /* 24*2^(n-1) */
    seq_t term1 = (seq_t)24 * two_exp;

    /* n as seq_t */
    seq_t n_seq = (seq_t)n;

    /* 11*n^2 */
    seq_t term2 = (seq_t)11 * n_seq * n_seq;

    /* 57*n */
    seq_t term3 = (seq_t)57 * n_seq;

    /* Sum all parts */
    seq_t result = term1 + term2 + term3 + (seq_t)52;
    return result;
}
