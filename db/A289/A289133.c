/*
 * A289133  a(n) is the number of odd integers divisible by 9 in ]2*(n-1)^2,
 *          2*n^2[.
 * https://oeis.org/A289133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #odd k such that 2*(n-1)^2 < 9*k < 2*n^2 computed as a(n)
 *            = oddCount( floor((2*n^2-1)/9) ) - oddCount( floor(2*(n-1)^2/9)
 *            ) where oddCount(x) = (x+1)/2 for x >= 0
 * Range:     u8 n <= 11, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is defined as 0 per OEIS listing */
    if (n == 0)
        return 0;

    /* Compute 2 * n^2 as a seq_t to avoid overflow */
    seq_t n_sq = (seq_t)n * (seq_t)n;      /* n^2 */
    seq_t two_n_sq = n_sq * 2;             /* 2*n^2 */

    /* Upper bound for k: k <= floor((2*n^2 - 1)/9) */
    seq_t U = (two_n_sq - 1) / 9;          /* floor((2*n^2 - 1)/9) */

    /* Lower bound for k: k >= floor(2*(n-1)^2 / 9) + 1 */
    idx_t m = n - 1;                       /* n-1, safe since n>0 */
    seq_t m_sq = (seq_t)m * (seq_t)m;      /* (n-1)^2 */
    seq_t two_m_sq = m_sq * 2;             /* 2*(n-1)^2 */
    seq_t L = two_m_sq / 9;                /* floor(2*(n-1)^2 / 9) */

    /* Count odd integers up to a bound: oddCount(x) = (x + 1)/2 */
    seq_t oddU = (U + 1) / 2;
    seq_t oddL = (L + 1) / 2;

    /* Final answer */
    return oddU - oddL;
}
