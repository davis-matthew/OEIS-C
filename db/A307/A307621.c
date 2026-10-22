/*
 * A307621  Number of cycles in the n-dipyramidal graph.
 * https://oeis.org/A307621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n - 1)*(2*n^2 - 4*n + 15)/6 + 1
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is assumed >= 1; arithmetic uses seq_t to avoid overflow. */
    seq_t n0 = (seq_t)n;
    seq_t n1 = n0 - 1;
    seq_t n2 = 2 * n0 * n0 - 4 * n0 + 15;
    return (n0 * n1 * n2) / 6 + 1;
}
