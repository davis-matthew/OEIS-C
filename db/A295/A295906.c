/*
 * A295906  Number of (not necessarily maximal) cliques in the n X n king
 *          graph.
 * https://oeis.org/A295906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(5*n^2 - 8*n + 4)
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t to avoid mixing idx_t / seq_t types */
    seq_t nn = (seq_t)n;
    seq_t term = 2 * (5 * nn * nn - 8 * nn + 4);
    return term;
}
