/*
 * A337898  Number of achiral colorings of the 6 square faces of a cube or the
 *          6 vertices of a regular octahedron using n or fewer colors.
 * https://oeis.org/A337898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(3*n^2 - 3*n + 4)/24
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = (seq_t)3 * nn * nn - (seq_t)3 * nn + (seq_t)4;
    return nn * (nn + (seq_t)1) * (nn + (seq_t)2) * term / (seq_t)24;
}
