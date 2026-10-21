/*
 * A250900  Number of (2+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 23/2 - 2^(n+4) + (7*3^(n+2))/2 + 2*n + 3*2^(n+3)*n + n^2
 *            (implemented without fractions: a(n) = 15 + 7*((3^(n+2)-1)/2) -
 *            2^(n+4) + 2*n + 3*2^(n+3)*n + n^2)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    /* Sequence is defined for n >= 1; return 0 for n == 0 to avoid UB. */
    if (n == 0) return (seq_t)0;

    /* Powers of 2 and 3 needed for the formula. */
    seq_t p3   = pow3(n + 2);  /* 3^(n+2) */
    seq_t p2_4 = pow2(n + 4);  /* 2^(n+4) */
    seq_t p2_3 = pow2(n + 3);  /* 2^(n+3) */

    /* 15 + 7*((3^(n+2)-1)/2)  (this is the integer equivalent of
       23/2 + (7*3^(n+2))/2) */
    seq_t half       = (p3 - (seq_t)1) >> 1;          /* (3^(n+2)-1)/2  */
    seq_t termA      = (seq_t)15 + half * (seq_t)7;   /* 15 + 7*half     */
    seq_t termC      = (seq_t)2 * n;                  /* 2*n             */
    seq_t termD      = (seq_t)3 * p2_3 * n;           /* 3*2^(n+3)*n     */
    seq_t termE      = (seq_t)n * n;                  /* n^2             */

    /* Sum of positive contributions minus the 2^(n+4) term. */
    seq_t pos_sum   = termA + termC + termD + termE;
    return pos_sum - p2_4;
}
