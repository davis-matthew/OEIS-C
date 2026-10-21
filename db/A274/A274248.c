/*
 * A274248  Row sums of A273751.
 * https://oeis.org/A274248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n^3 - 3*n^2 + 10*n + 3*mod(n, 2))/24
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 753, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t tn = (seq_t)n;
    seq_t tn2 = tn * tn;
    seq_t tn3 = tn2 * tn;
    seq_t num = (seq_t)14 * tn3 - (seq_t)3 * tn2 + (seq_t)10 * tn + (seq_t)3 * ((seq_t)n & 1);
    return num / (seq_t)24;
}
