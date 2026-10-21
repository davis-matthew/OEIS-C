/*
 * A250606  Number of (n+1) X (3+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 64*2^(n-1) + 5*n^2 + 4*n - 16   (n ≥ 1)
 * This uses the provided pow2 function to compute 2^(n-1).
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* term = 5*n^2 + 4*n */
    seq_t n_seq = (seq_t)n;
    seq_t term = (seq_t)5 * n_seq * n_seq + (seq_t)4 * n_seq;

    /* pow_term = 64*2^(n-1) - 16 */
    seq_t pow_term = pow2(n - 1);
    pow_term = (seq_t)64 * pow_term;
    pow_term -= (seq_t)16;

    return pow_term + term;
}
