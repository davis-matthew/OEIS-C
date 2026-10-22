/*
 * A347535  Number of minimum dominating sets in the complete bipartite graph
 *          K_n,n.
 * https://oeis.org/A347535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 for n > 2; a(1) = 2; a(2) = 6
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) { return (seq_t)0; } /* offset starts at 1, but guard */
    if (n == 1) { return (seq_t)2; }
    if (n == 2) { return (seq_t)6; }
    return (seq_t)n * (seq_t)n;
}
