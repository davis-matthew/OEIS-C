/*
 * A243206  Number of 3-matchings of the n X n grid graph.
 * https://oeis.org/A243206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^6 - 12*n^5 - 30*n^4 + 116*n^3 + 14*n^2 - 272*n +
 *            156) / 3 * for n >= 3; a(n) = 0 for n < 3. This computes the
 *            number of 3‑matchings * (placements of three dominoes) on an n×n
 *            grid
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 37, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) {
        return (seq_t)0;
    }
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n2 * n2;
    seq_t n5 = n4 * N;
    seq_t n6 = n4 * n2;          /* n^6 = n^4 * n^2 */
    seq_t num = 4 * n6 - 12 * n5 - 30 * n4 + 116 * n3 + 14 * n2 - 272 * N + 156;
    return num / (seq_t)3;
}
