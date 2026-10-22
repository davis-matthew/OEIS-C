/*
 * A392173  Total number of vertices in the graph (see A392172) formed when n
 *          points are placed in general position on each edge of a square and
 *          a chord is drawn from each point to the 3*n points on the other
 *          three sides.
 * https://oeis.org/A392173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (17*n^4 - 18*n^3 + 3*n^2 + 8*n + 8)/2
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 149, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;          /* n^2   */
    seq_t n3 = n2 * n_seq;             /* n^3   */
    seq_t n4 = n3 * n_seq;             /* n^4   */
    seq_t numerator = (seq_t)17 * n4
                     - (seq_t)18 * n3
                     + (seq_t)3  * n2
                     + (seq_t)8  * n_seq
                     + (seq_t)8;
    return numerator / 2;
}
