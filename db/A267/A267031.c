/*
 * A267031  a(n) = (32*n^3 - 2*n)/3.
 * https://oeis.org/A267031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (32*n^3 - 2*n)/3 (OEIS A267031)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 564, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;          /* n^2   */
    seq_t nn3 = nn2 * nn;         /* n^3   */
    seq_t term = (32 * nn3) - (2 * nn);
    return term / 3;
}
