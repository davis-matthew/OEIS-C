/*
 * A279437  Number of ways to place 3 points on an n X n square grid so that
 *          no more than 2 points are on a vertical or horizontal straight
 *          line.
 * https://oeis.org/A279437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 - 5*n^4 + 6*n^3 - 2*n^2)/6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 53, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */
    seq_t n6 = n4 * n2;          /* n^6 */

    seq_t num = n6 - (seq_t)5 * n4 + (seq_t)6 * n3 - (seq_t)2 * n2;
    return num / (seq_t)6;
}
