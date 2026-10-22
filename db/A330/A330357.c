/*
 * A330357  a(n) = (2*n^2 + 9 - (-1)^n)/4.
 * https://oeis.org/A330357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 + 9 - (-1)^n)/4
 * Range:     u8 n <= 11, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n * (seq_t)n;
    seq_t numerator = (seq_t)2 * nn + ((n & 1) ? (seq_t)10 : (seq_t)8);
    return numerator / 4;
}
