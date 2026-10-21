/*
 * A279438  Number of ways to place 4 points on an n X n square grid so that
 *          no more than 2 points are on a vertical or horizontal straight
 *          line.
 * https://oeis.org/A279438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 - 14*n^6 + 30*n^5 - 17*n^4 - 6*n^3 + 6*n^2)/24
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 23, u64 n <= 380, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128-bit arithmetic for intermediate values to avoid overflow
       on 64-bit builds while still fitting n^8 for n up to about 10^4. */
    unsigned __int128 N      = n;
    unsigned __int128 N2     = N * N;              /* n^2   */
    unsigned __int128 N3     = N2 * N;             /* n^3   */
    unsigned __int128 N4     = N3 * N;             /* n^4   */
    unsigned __int128 N5     = N4 * N;             /* n^5   */
    unsigned __int128 N6     = N5 * N;             /* n^6   */
    unsigned __int128 N8     = N6 * N2;            /* n^8   */

    unsigned __int128 res = N8
        - (unsigned __int128)14 * N6
        + (unsigned __int128)30 * N5
        - (unsigned __int128)17 * N4
        - (unsigned __int128)6  * N3
        + (unsigned __int128)6  * N2;

    res /= 24;

    return (seq_t)res;
}
