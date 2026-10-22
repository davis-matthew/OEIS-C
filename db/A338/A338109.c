/*
 * A338109  a(n)/A002939(n+1) is the Kirchhoff index of the join of the
 *          disjoint union of two complete graphs on n vertices with the empty
 *          graph on n+1 vertices.
 * https://oeis.org/A338109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 10*n + 31*n^2 + 18*n^3
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                /* cast index to the result type          */
    seq_t term1 = 10 * nn;              /* 10 * n                                 */
    seq_t term2 = 31 * nn * nn;         /* 31 * n^2                               */
    seq_t term3 = 18 * nn * nn * nn;    /* 18 * n^3                               */
    return 1 + term1 + term2 + term3;
}
