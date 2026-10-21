/*
 * A274772  Zero together with the partial sums of A056640.
 * https://oeis.org/A274772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^4+8*n^3+2*n^2+4*n+3*(1-(-1)^n))/24
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 260, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t n2 = x * x;          /* n^2 */
    seq_t n3 = n2 * x;         /* n^3 */
    seq_t n4 = n3 * x;         /* n^4 */
    seq_t num = 4 * n4 + 8 * n3 + 2 * n2 + 4 * x;
    if (n & 1) num += 6;       /* add 6 for odd n */
    return num / 24;
}
