/*
 * A339609  Consider a triangle drawn on the perimeter of a triangular lattice
 *          with side length n. a(n) is the number of regions inside the
 *          triangle after drawing unit circles centered at each lattice point
 *          inside the triangle.
 * https://oeis.org/A339609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^2 - 21*n + 24)/2 for n >= 4 (a(1) = 0, a(2) = 0,
 *            a(3) = 4)
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1 || n == 2) {
        return 0;
    }
    if (n == 3) {
        return 4;
    }
    seq_t nn = (seq_t)n;
    seq_t val = ((seq_t)5 * nn * nn - (seq_t)21 * nn + (seq_t)24) / (seq_t)2;
    return val;
}
