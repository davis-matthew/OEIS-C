/*
 * A344679  Number of 2-matchings of the n-th centered square grid graph.
 * https://oeis.org/A344679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-2)*(4*n^3-8*n^2+n+4) for n > 1; a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 153, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2) return 0;
    seq_t N = n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t poly = (seq_t)4 * n3 - (seq_t)8 * n2 + N + (seq_t)4;
    return (seq_t)2 * (N - (seq_t)2) * poly;
}
