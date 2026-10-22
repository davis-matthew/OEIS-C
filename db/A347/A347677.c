/*
 * A347677  Number of Baxter matrices of size 3 X n that contain n+2 1's.
 * https://oeis.org/A347677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^2 - 13*n + 11 for n >= 3; a(1)=1; a(2)=8
 * Range:     u8 n <= 7, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    seq_t nn = (seq_t)n;
    seq_t term1 = (seq_t)6 * nn * nn;
    seq_t term2 = (seq_t)13 * nn;
    return term1 - term2 + (seq_t)11;
}
