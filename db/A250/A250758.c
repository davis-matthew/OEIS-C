/*
 * A250758  Number of (3+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((173 * n^2 + 329 * n) / 2) + 81
 * Range:     u8 none, u16 n <= 18, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (a(1) is the first term) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The formula is integer for every integer n */
    /* Cast n to seq_t, then perform the arithmetic. */
    seq_t nseq = (seq_t)n;
    seq_t term = (seq_t)173 * nseq * nseq + (seq_t)329 * nseq;
    term = term / 2;
    term += 81;
    return term;
}
