/*
 * A308603  Number of (not necessarily maximal) cliques in the n-dipyramidal
 *          graph.
 * https://oeis.org/A308603
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3) = 24, a(n) = 6*n + 3 for n > 3
 * Range:     u8 n <= 42, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3)
        return (seq_t)24;
    else
        return ((seq_t)6 * (seq_t)n) + (seq_t)3;
}
