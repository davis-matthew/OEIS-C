/*
 * A270869  a(n) = n^5 + 4*n^4 + 13*n^3 + 23*n^2 + 25*n + 3.
 * https://oeis.org/A270869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 + 4*n^4 + 13*n^3 + 23*n^2 + 25*n + 3
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 83, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;     /* cast index to seq_t safely */
    seq_t n1 = sn;
    seq_t n2 = n1 * n1;
    seq_t n3 = n2 * n1;
    seq_t n4 = n3 * n1;
    seq_t n5 = n4 * n1;
    return n5 + (seq_t)4 * n4 + (seq_t)13 * n3 + (seq_t)23 * n2 + (seq_t)25 * n1 + (seq_t)3;
}
