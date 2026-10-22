/*
 * A365892  a(n) is the index of the n-th term of A365876 that includes at
 *          least one equation with at least one integer solution.
 * https://oeis.org/A365892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 1, a(n) = ((n + 1)^2 - (-1)^n * (n - 1)) / 2
 *            for n >= 2 (equivalently a(2k)=2k^2+k+1, a(2k+1)=2k^2+5k+2 for
 *            k>=1)
 * Range:     u8 n <= 14, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t nn = (seq_t)n;
    seq_t term1 = (nn + 1) * (nn + 1);   /* (n + 1)^2 */
    seq_t term2 = (nn - 1);              /* (n - 1) */

    seq_t numerator;
    if ((n & 1) == 0)               /* n even: (-1)^n = 1  */
        numerator = term1 - term2; /* (n+1)^2 - (n-1)   */
    else                            /* n odd:  (-1)^n = -1 */
        numerator = term1 + term2; /* (n+1)^2 + (n-1)   */

    return numerator / 2;          /* integer division is exact */
}
