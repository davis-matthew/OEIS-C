/*
 * A397979  Upper (1/2,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A397979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*(5*n + 1) / 4) (upper (1/2,1) midsequence of
 *            triangular numbers and squares, OEIS A397979)
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for type‐safe arithmetic */
    seq_t nn = (seq_t)n;
    /* Compute ceil( n*(5*n + 1) / 4 ) as (n*(5*n + 1) + 3) / 4 */
    seq_t val = nn * (nn * 5 + 1) + 3;
    val /= 4;
    return val;
}
