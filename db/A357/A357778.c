/*
 * A357778  Maximum number of edges in a 5-degenerate graph with n vertices.
 * https://oeis.org/A357778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)/2 for n < 7, otherwise a(n) = 5*n - 15
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the maximum number of edges in a 5‑degenerate graph with n vertices. */
seq_t a(idx_t n)
{
    if (n < 7) {
        /* For n < 7 the graph is complete */
        seq_t nn = n;
        return nn * (nn - 1) / 2;
    } else {
        /* For n ≥ 7 a maximal 5‑degenerate graph has 5n−15 edges */
        return (seq_t)n * 5 - 15;
    }
}
