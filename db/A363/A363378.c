/*
 * A363378  Third Lie-Betti number of a cycle graph on n vertices.
 * https://oeis.org/A363378
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+11)*(n-2)/6 for n>=6; a(3)=12, a(4)=25, a(5)=41
 * Range:     u8 n <= 9, u16 n <= 70, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;            /* values below the offset are undefined */
    if (n == 3)
        return (seq_t)12;
    if (n == 4)
        return (seq_t)25;
    if (n == 5)
        return (seq_t)41;

    seq_t sn = (seq_t)n;
    return sn * (sn + 11) * (sn - 2) / 6;
}
