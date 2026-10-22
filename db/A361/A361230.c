/*
 * A361230  Third Lie-Betti number of a path graph on n vertices.
 * https://oeis.org/A361230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 9*n^2 - 40*n + 48)/6 for n >= 3, a(1)=0, a(2)=1
 * Range:     u8 n <= 5, u16 n <= 37, u32 n <= 1838, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 1;
    seq_t nn = (seq_t)n;
    seq_t term = nn * nn * nn
               + (seq_t)9 * nn * nn
               - (seq_t)40 * nn
               + (seq_t)48;
    return term / 6;
}
