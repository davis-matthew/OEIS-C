/*
 * A288918  Number of 4-cycles in the n X n king graph.
 * https://oeis.org/A288918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^2 - 34*n + 23 for n > 1, a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 75, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    seq_t nn = (seq_t)n;
    return (seq_t)12 * nn * nn - (seq_t)34 * nn + (seq_t)23;
}
