/*
 * A338588  a(n)/A002939(n+1) is the Kirchhoff index of the disjoint union of
 *          two complete graphs each on n and n+1 vertices with the empty
 *          graph on n+1 vertices.
 * https://oeis.org/A338588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*n^3 + 37*n^2 + 20*n + 2
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 619, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    return (seq_t)18 * n3 + (seq_t)37 * n2 + (seq_t)20 * nn + (seq_t)2;
}
