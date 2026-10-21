/*
 * A269658  Number of length-5 0..n arrays with no adjacent pair x,x+1
 *          repeated.
 * https://oeis.org/A269658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^5 - 3*n*(n+1)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* n */
    seq_t n1 = nn + 1;            /* n+1 */

    /* compute (n+1)^5 */
    seq_t pow_n1 = n1 * n1;       /* (n+1)^2 */
    pow_n1 = pow_n1 * pow_n1;     /* (n+1)^4 */
    pow_n1 = pow_n1 * n1;         /* (n+1)^5 */

    /* compute 3*n*(n+1) */
    seq_t sub = 3 * nn * n1;

    return pow_n1 - sub;
}
