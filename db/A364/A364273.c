/*
 * A364273  Number of chordless cycles (with length >= 4) in the complement of
 *          the n X n X n grid graph.
 * https://oeis.org/A364273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3/2*(16-108*n+155*n^2-57*n^3+3*n^4-6*n^5+3*n^6), a(1) =
 *            0, a(2) = 6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset 1 base */
    if (n <= 2) {
        if (n == 1) return (seq_t)0;
        return (seq_t)6;   /* n == 2 */
    }

    /* Use 128‑bit intermediate to avoid overflow for 64‑bit seq_t */
    __int128 ns = ( __int128) n;
    __int128 n2 = ns * ns;
    __int128 n3 = n2 * ns;
    __int128 n4 = n3 * ns;
    __int128 n5 = n4 * ns;
    __int128 n6 = n5 * ns;

    __int128 poly = (__int128)16
                   - (__int128)108 * ns
                   + (__int128)155 * n2
                   - (__int128)57  * n3
                   + (__int128)3   * n4
                   - (__int128)6   * n5
                   + (__int128)3   * n6;

    poly /= (__int128)2;   /* polynomial is always even */
    poly *= (__int128)3;   /* multiply by 3 */

    return (seq_t)poly;
}
