/*
 * A304579  a(n) = (n^2 + 1)*(n^2 + 2).
 * https://oeis.org/A304579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 1)*(n^2 + 2)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;
    return (sq + 1) * (sq + 2);
}
