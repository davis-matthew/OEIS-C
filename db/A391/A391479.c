/*
 * A391479  Maximum number of vertices in a uniquely traceable induced
 *          subgraph of the n X n grid graph.
 * https://oeis.org/A391479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(0)=0, a(1)=1, a(2)=3, a(3)=7, a(n)=n*(n-1)+2 for n>=4
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 3) return 7;
    seq_t nn = (seq_t)n;
    return nn * (nn - 1) + 2;
}
