/*
 * A269412  Number of length-6 0..n arrays with no repeated value greater than
 *          or equal to the previous repeated value.
 * https://oeis.org/A269412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 6*n^5 + 8*n^4 + (5*n^3 + n)/3 - n^2 (valid for all
 *            integer n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle indices less than 1 gracefully; a(0) is not defined in OEIS, but return 0 here */
    if (n == 0)
        return 0;

    seq_t N   = (seq_t)n;
    seq_t N2  = N * N;           /* n^2   */
    seq_t N3  = N2 * N;          /* n^3   */
    seq_t N4  = N3 * N;          /* n^4   */
    seq_t N5  = N4 * N;          /* n^5   */
    seq_t N6  = N5 * N;          /* n^6   */

    seq_t term1 = N6;                                 /* n^6 */
    seq_t term2 = 6 * N5;                             /* 6*n^5 */
    seq_t term3 = 8 * N4;                             /* 8*n^4 */
    seq_t temp  = 5 * N3 + N;                         /* 5*n^3 + n  */
    seq_t term4 = temp / 3;                           /* (5*n^3 + n)/3  */
    seq_t res   = term1 + term2;                      /* first two terms */
    res += term3;                                     /* add third term */
    res += term4;                                     /* add fourth term */
    res -= N2;                                        /* subtract n^2 */

    return res;
}
