/*
 * A400235  a(n) is the number of boundary lattice points minus the number of
 *          interior lattice points of the prism with vertices (0,0,0),
 *          (n,0,0), (0,n,0), (0,0,n), (n,0,n) and (0,n,n).
 * https://oeis.org/A400235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-n^3 + 12*n^2 - 5*n + 6)/2
 * Range:     i8 n <= 13, i16 n <= 44, i32 n <= 1628, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return ((-x*x*x) + (12*x*x) - (5*x) + 6) / 2;
}
