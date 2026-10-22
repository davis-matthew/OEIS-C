/*
 * A328152  a(n) is the number of squares of side length greater than 1 having
 *          vertices at the points of an n X n grid of dots.
 * https://oeis.org/A328152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4/12 - 13*n^2/12 + 2*n - 1 (OEIS A328152) Equivalent:
 *            a(n) = n^2*(n^2-1)/12 - (n-1)^2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;
    seq_t term = n4
               - (seq_t)13 * n2
               + (seq_t)24 * nn
               - (seq_t)12;
    return term / (seq_t)12;
}
