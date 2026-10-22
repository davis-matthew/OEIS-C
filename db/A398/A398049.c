/*
 * A398049  Upper (3/4,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((11*n^2 + 3*n)/8) = (11*n^2 + 3*n + 7)/8
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term1 = (seq_t)11u * nn * nn;   /* 11*n^2 */
    seq_t term2 = (seq_t)3u * nn;         /* 3*n   */
    return (term1 + term2 + (seq_t)7u) / (seq_t)8u;
}
