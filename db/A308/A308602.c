/*
 * A308602  Number of (not necessarily maximal) cliques in the n-cycle graph.
 * https://oeis.org/A308602
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 1 for n > 3, a(3) = 8
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)8;
    if (n > 3)  return ((seq_t)n) * 2 + 1;
    /* for n < 3 the sequence is not defined; return 0 */
    return 0;
}
