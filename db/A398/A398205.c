/*
 * A398205  Upper (-1/3,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( n^2 - tri(n)/3 ) = ceil( (5*n^2 - n)/6 ) Equivalent
 *            integer formula: a(n) = (5*n*n - n + 5)/6 where tri(n) =
 *            n*(n+1)/2
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    /* compute numerator = 5n^2 - n + 5 */
    seq_t num = 5 * nn * nn - nn + 5;
    /* divide by 6 (integer division truncates because num is non‑negative) */
    return num / 6;
}
