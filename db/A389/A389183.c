/*
 * A389183  Number of mutual-visibility sets in the n-ladder graph.
 * https://oeis.org/A389183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(13 + 8*n + 2*n^2 + n^3)/6
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Off‑by‑one: the OEIS offset is 1, so a(1) is the first
       value. The function is defined for n>=1, but we guard
       against zero just in case.                     */
    if (n == 0) return 0;
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;
    seq_t n3  = n2 * N;
    seq_t inner = (seq_t)13 + (seq_t)8 * N + (seq_t)2 * n2 + n3;
    return N * inner / 6;
}
