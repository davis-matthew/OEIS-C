/*
 * A361185  Number of chordless cycles in the n X n rook complement graph.
 * https://oeis.org/A361185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 2)*(n - 1)^2*n^2*(6*n - 13)/12
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 46, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the graph has no chordless cycles. */
    if (n < 3)
        return 0;

    /* Use an unsigned 128‑bit type to avoid intermediate overflow. */
    unsigned __int128 nn   = n;
    unsigned __int128 num  = (nn - 2) * (nn - 1) * (nn - 1) * nn * nn * (6 * nn - 13);
    num /= 12;

    return (seq_t)num;
}
