/*
 * A250775  Number of (7+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 32*2^(n-1) + 57*n^2 + 247*n + 240
 * (valid for all n >= 1)
 */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n) {
    /* n is guaranteed to be >= 1 by the sequence definition */
    /* compute 32 * 2^(n-1) */
    seq_t term = pow2(n - 1);
    term *= (seq_t)32;

    /* add 57*n^2 */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    term += n_sq * (seq_t)57;

    /* add 247*n */
    term += (seq_t)247 * n;

    /* add constant 240 */
    term += (seq_t)240;

    return term;
}
