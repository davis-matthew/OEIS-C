/*
 * A273321  Wiener index of graph of b.c.c. unit cells in a line = Sum of
 *          distances in a b.c.c. row graph.
 * https://oeis.org/A273321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (25*n^3 + 60*n^2 + 71*n + 36) / 3, n >= 1
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 623, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1, but safe guard */
    seq_t N = (seq_t)n;
    seq_t term = (seq_t)25 * N * N * N;
    term += (seq_t)60 * N * N;
    term += (seq_t)71 * N;
    term += (seq_t)36;
    return term / (seq_t)3;
}
