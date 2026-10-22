/*
 * A374709  a(n) = n*(6*n^4 + 8*n^3 + 1 - (-1)^n)/16.
 * https://oeis.org/A374709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(6*n^4 + 8*n^3 + 1 - (-1)^n)/16
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;          /* n^2 */
    seq_t n3 = n2 * tn;          /* n^3 */
    seq_t n4 = n3 * tn;          /* n^4 */
    seq_t term = 6 * n4 + 8 * n3; /* 6*n^4 + 8*n^3 */
    if (n & 1) {
        term += 2;               /* add parity term 2 if n is odd */
    }
    seq_t num = tn * term;       /* n * term */
    return num >> 4;             /* divide by 16 (exact division) */
}
