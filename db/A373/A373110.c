/*
 * A373110  Number of distinct circles that can be constructed from the 4
 *          vertices and the equally spaced 4*n points placed on the sides of
 *          a square when every pair of the 4 + 4*n points are connected by a
 *          circle and where the points lie at the ends of the circle's
 *          diameter.
 * https://oeis.org/A373110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (14*n^2 + 21*n + (n even ? 10 : 9))/2
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = (seq_t)14 * nn * nn + (seq_t)21 * nn + (seq_t)(n & 1 ? 9 : 10);
    return term / 2;
}
