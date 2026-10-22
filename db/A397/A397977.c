/*
 * A397977  Upper (1,3/2) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A397977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( n*(4*n+1)/2 )
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = (nn << 2) + (seq_t)1;   /* 4*n + 1 */
    seq_t num = nn * t;                /* n*(4*n+1) */

    if ((num & (seq_t)1) == 0)          /* even numerator */
        return num >> 1;                /* divide by 2 */
    else
        return (num + (seq_t)1) >> 1;   /* ceil for odd numerator */
}
