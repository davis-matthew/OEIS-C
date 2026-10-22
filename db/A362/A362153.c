/*
 * A362153  Number of skew shapes in a 3 X n rectangle with no empty rows or
 *          columns.
 * https://oeis.org/A362153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 8*n^3 + 11*n^2 - 20*n + 12)/12
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 306, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t num = n4 + 8 * n3 + 11 * n2 + 12 - 20 * nn;
    return num / (seq_t)12;
}
