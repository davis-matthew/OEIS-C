/*
 * A344866  Number of polygons formed when every pair of vertices of a regular
 *          (2n-1)-gon are joined by an infinite line.
 * https://oeis.org/A344866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^4 - 11*n^3 + 23*n^2 - 21*n + 7
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 216, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t tn2 = tn * tn;
    seq_t tn3 = tn2 * tn;
    seq_t tn4 = tn2 * tn2;
    seq_t res = 2 * tn4 - 11 * tn3 + 23 * tn2 - 21 * tn + 7;
    return res;
}
