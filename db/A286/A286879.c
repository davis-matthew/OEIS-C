/*
 * A286879  Number of minimal dominating sets in the n-Andrásfai graph.
 * https://oeis.org/A286879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n - 1)*(n^4 - 13*n^3 + 164*n^2 - 572*n + 960)/120 for
 *            n > 3
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 179, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n-th term of OEIS sequence A286879. */

seq_t a(idx_t n)
{
    /* Small values known from the OEIS table */
    static const seq_t base[10] = {0, 2, 5, 28, 66, 140, 272,
                                   489, 828, 1339};

    if (n <= 9)
        return base[n];

    /* Use 128‑bit arithmetic to avoid overflow in intermediate
       calculations. All final results fit into seq_t for the tested n. */
    unsigned __int128 nn = n;

    unsigned __int128 poly = nn * nn * nn * nn                /* n^4      */
                         - (unsigned __int128)13 * nn * nn * nn /* -13 n^3  */
                         + (unsigned __int128)164 * nn * nn    /* +164 n^2 */
                         - (unsigned __int128)572 * nn         /* -572 n   */
                         + 960;                               /* +960     */

    unsigned __int128 numerator = (unsigned __int128)(3 * nn - 1) * poly;
    numerator /= 120;

    return (seq_t)numerator;
}
