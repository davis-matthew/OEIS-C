/*
 * A302351  Hyper-Wiener index of body-centered cubic grid cells in a row.
 * https://oeis.org/A302351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (25*n^4 + 105*n^3 + 143*n^2 + 171*n + 108)/6
 * Range:     u8 none, u16 n <= 6, u32 n <= 177, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t N2 = N * N;          /* n^2 */
    seq_t N3 = N2 * N;         /* n^3 */
    seq_t N4 = N3 * N;         /* n^4 */
    seq_t t = 25 * N4;
    t += 105 * N3;
    t += 143 * N2;
    t += 171 * N;
    t += 108;
    return t / 6;
}
