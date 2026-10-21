/*
 * A250770  Number of (2+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*2^(n-1) + 4*n + 2 (offset 1)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For the sequence definition n >= 1.  Lower indices are undefined. */
    if (n < 1) return (seq_t)0;

    /* 12 * 2^(n-1) */
    seq_t term = pow2(n - 1);          /* 2^(n-1) */
    term *= (seq_t)12;                 /* *12 */

    /* + 4*n + 2 */
    term += (seq_t)4 * (seq_t)n;       /* +4*n */
    term += (seq_t)2;                  /* +2 */

    return term;
}
