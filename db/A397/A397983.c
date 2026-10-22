/*
 * A397983  Upper (2/3,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A397983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((4*n^2 + n)/3) = (4*n^2 + n + 2)/3
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                  /* convert index to seq_t */
    seq_t val = (seq_t)4 * nn * nn;       /* 4*n^2   */
    val += nn;                            /* + n     */
    val += (seq_t)2;                      /* + 2     */
    val /= (seq_t)3;                      /* ceil division */
    return val;
}
