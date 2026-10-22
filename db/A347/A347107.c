/*
 * A347107  a(n) = Sum_{1 <= i < j <= n} j^3*i^3.
 * https://oeis.org/A347107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n-1)*(21*n^5 + 36*n^4 - 21*n^3 - 48*n^2 + 8) /
 *            672
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the sum is empty */
    if (n <= 1)
        return 0;

    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;       /* n^2   */
    seq_t n3  = n2 * N;      /* n^3   */
    seq_t n4  = n2 * n2;     /* n^4   */
    seq_t n5  = n4 * N;      /* n^5   */

    /* Polynomial part: 21*n^5 + 36*n^4 - 21*n^3 - 48*n^2 + 8 */
    seq_t poly = 21 * n5 + 36 * n4 - 21 * n3 - 48 * n2 + 8;

    /* Final numerator: n*(n+1)*(n-1)*poly */
    seq_t numerator = N * (N + 1) * (N - 1) * poly;

    /* Integer division is exact by definition */
    return numerator / 672;
}
