/*
 * A270871  a(n) = n^7 + 6*n^6 + 26*n^5 + 73*n^4 + 152*n^3 + 222*n^2 + 203*n +
 *          8.
 * https://oeis.org/A270871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 + 6*n^6 + 26*n^5 + 73*n^4 + 152*n^3 + 222*n^2 + 203*n
 *            + 8
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 22, u64 n <= 564, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;   /* cast index to seq_t first */
    seq_t res = 1;
    res = res * x + 6;
    res = res * x + 26;
    res = res * x + 73;
    res = res * x + 152;
    res = res * x + 222;
    res = res * x + 203;
    res = res * x + 8;
    return res;
}
