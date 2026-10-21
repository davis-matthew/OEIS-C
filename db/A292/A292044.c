/*
 * A292044  Wiener index of the n-halved cube graph.
 * https://oeis.org/A292044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0; a(2)=1; a(n)=2^(2*n-5)*n for n>2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    idx_t exp = 2*n - 5;          /* n>2 so exp>=1 */
    seq_t val = pow2(exp);        /* 2^(2*n-5) */
    val *= (seq_t)n;              /* multiply by n */
    return val;
}
