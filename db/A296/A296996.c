/*
 * A296996  Number of nonequivalent (mod D_8) ways to place 3 points on an n X
 *          n point grid so that no point is equally distant from two other
 *          points on the same row or the same column.
 * https://oeis.org/A296996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 - 3*n^4 + 5*n^3 - 4*n^2 + 4*n)/48 for n even * a(n)
 *            = (n^6 - 3*n^4 + 13*n^3 - 22*n^2 + 11*n)/48 for n odd
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 75, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    /* n == 1 gives 0; for n >= 2 the subtractions are safe (do not underflow). */
    if (N <= 1) return 0;

    seq_t N2 = N * N;           /* n^2 */
    seq_t N3 = N2 * N;          /* n^3 */
    seq_t N4 = N3 * N;          /* n^4 */
    seq_t N6 = N3 * N3;         /* n^6 */

    seq_t num;
    if ((n & 1) == 0) {         /* n even */
        num = N6 - 3 * N4 + 5 * N3 - 4 * N2 + 4 * N;
    } else {                    /* n odd */
        num = N6 - 3 * N4 + 13 * N3 - 22 * N2 + 11 * N;
    }

    return num / 48;
}
