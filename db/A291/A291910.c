/*
 * A291910  Number of 4-cycles in the n X n rook complement graph.
 * https://oeis.org/A291910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n-1)^2*n^2*(-4 + 5*n - 4*n^2 + n^3)/8
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n <= 2 gives 0 because (n-1)^2 == 0 or (n-2) == 0 */
    if (n <= (idx_t)2)
        return (seq_t)0;

    seq_t N = (seq_t)n;

    /* (n-2)*(n-1)^2*n^2 part */
    seq_t term = N - (seq_t)2;
    term *= N - (seq_t)1;
    term *= N - (seq_t)1;
    term *= N;
    term *= N;

    /* integer polynomial part, computed in 64‑bit signed */
    int64_t n64 = (int64_t)n;
    int64_t n2  = n64 * n64;
    int64_t n3  = n2 * n64;
    int64_t poly_i = -4 + 5 * n64 - 4 * n2 + n3;
    seq_t poly = (seq_t)poly_i;

    term *= poly;
    term /= (seq_t)8;

    return term;
}
