/*
 * A300624  Figurate numbers based on the 11-dimensional regular convex
 *          polytope called the 11-dimensional cross-polytope, or
 *          11-dimensional hyperoctahedron.
 * https://oeis.org/A300624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(14175 + 83754*n^2 + 50270*n^4 + 7392*n^6 + 330*n^8 +
 *            4*n^10)) / 155925
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 19, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* Use 128‑bit arithmetic for intermediate size */
    unsigned __int128 nn = n;
    unsigned __int128 n2 = nn * nn;          /* n^2   */
    unsigned __int128 n4 = n2 * n2;          /* n^4   */
    unsigned __int128 n6 = n4 * n2;          /* n^6   */
    unsigned __int128 n8 = n4 * n4;          /* n^8   */
    unsigned __int128 n10 = n8 * n2;         /* n^10  */
    unsigned __int128 poly = (unsigned __int128)14175
                           + (unsigned __int128)83754 * n2
                           + (unsigned __int128)50270 * n4
                           + (unsigned __int128)7392 * n6
                           + (unsigned __int128)330 * n8
                           + (unsigned __int128)4   * n10;
    unsigned __int128 num  = nn * poly;      /* n * poly */
    seq_t result = (seq_t)(num / 155925);    /* division by 155925 */
    return result;
}
