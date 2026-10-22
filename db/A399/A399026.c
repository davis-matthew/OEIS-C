/*
 * A399026  a(n) is the number of inequivalent ways of choosing 4 points on an
 *          n X n grid that form a square up to rotations and reflections.
 * https://oeis.org/A399026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/96)*(n-1)*(n^3+n^2+39*n-9) for odd n, a(n) =
 *            (1/96)*n*(n^3+32*n-24) for even n
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 279, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    const seq_t inv96 = (seq_t)96;
    if (n & 1) {   /* odd n */
        seq_t n3 = N * N * N;          /* N^3 */
        seq_t n2 = N * N;              /* N^2 */
        seq_t inner = n3 + n2 + ((seq_t)39 * N) - (seq_t)9;
        seq_t num   = (N - (seq_t)1) * inner;
        return num / inv96;
    } else {        /* even n */
        seq_t n3 = N * N * N;          /* N^3 */
        seq_t inner = n3 + ((seq_t)32 * N) - (seq_t)24;
        seq_t num   = N * inner;
        return num / inv96;
    }
}
