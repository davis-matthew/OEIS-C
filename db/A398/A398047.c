/*
 * A398047  Upper (1/4,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*n + ( (n*(n+1)/2) + 3 ) / 4
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           (i.e., a(n) = n^2 + ceil( T(n)/4 ), T(n) = n(n+1)/2) */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                     /* cast index to seq_t */
    seq_t tn = nn * (nn + 1) / 2;            /* triangular number T(n) */
    seq_t sq = nn * nn;                      /* square n^2 */
    seq_t ceil_t4 = (tn + 3) / 4;            /* ceil(T(n)/4) = (T(n)+3)/4 */
    return sq + ceil_t4;                     /* final term a(n) */
}
