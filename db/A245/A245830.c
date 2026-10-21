/*
 * A245830  The Szeged index of a benzenoid consisting of a linear chain of n
 *          hexagons.
 * https://oeis.org/A245830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (44*n^3 + 72*n^2 + 43*n + 3)/3
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 663, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset starts at 1; return 0 for n == 0. */
    if (n == 0)
        return 0;

    /* Perform calculations in a wider type to avoid overflow. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 val = 44u * nn * nn * nn
                          + 72u * nn * nn
                          + 43u * nn
                          + 3u;
    val /= 3u;
    return (seq_t)val;
}
