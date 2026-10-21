/*
 * A299262  Partial sums of A299256.
 * https://oeis.org/A299262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0; a(n) = (6*n^3 + 9*n^2 + 2*n + (n%2?11:12)) /
 *            4 for n > 0
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 964, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t add = (nn & 1) ? (seq_t)11 : (seq_t)12;
    seq_t val = (seq_t)6 * n3 + (seq_t)9 * n2 + (seq_t)2 * nn + add;
    return val / (seq_t)4;
}
