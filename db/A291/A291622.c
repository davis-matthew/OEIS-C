/*
 * A291622  Number of irredundant sets in the n X n rook complement graph.
 * https://oeis.org/A291622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(2^n) + (5*n^6 - 21*n^5 + 47*n^4 - 51*n^3 + 8*n^2 -
 *            24*n + 12)/12 (n>3); initial values: a(1)=2, a(2)=9, a(3)=94
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1 – behaviour for 0 is irrelevant */
    if (n == 1) return 2;
    if (n == 2) return 9;
    if (n == 3) return 94;

    seq_t N = n;                        /* convert index to seq_t */
    /* term1 = 2 * N * 2^n */
    seq_t term1 = (seq_t)2 * N * pow2(n);

    /* polynomial part */
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;
    seq_t N5 = N4 * N;
    seq_t N6 = N5 * N;

    seq_t r = (seq_t)12;
    r = (seq_t)5 * N6 + r;
    r -= (seq_t)21 * N5;
    r += (seq_t)47 * N4;
    r -= (seq_t)51 * N3;
    r += (seq_t)8 * N2;
    r -= (seq_t)24 * N;
    r /= (seq_t)12;

    return term1 + r;
}
