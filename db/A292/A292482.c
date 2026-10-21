/*
 * A292482  p-INVERT of the odd positive integers, where p(S) = (1 - S)^2.
 * https://oeis.org/A292482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(1)=9, a(2)=32, a(3)=112; for n>=4,
 *            a(n)=6*a(n-1)-9*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 2;
    if (n == 1) return 9;
    if (n == 2) return 32;
    if (n == 3) return 112;

    seq_t a_prev2 = 32;      /* a(n-2) when n==4 */
    seq_t a_prev1 = 112;     /* a(n-1) when n==4 */
    seq_t a_curr = 0;

    for (idx_t k = 4; k <= n; ++k) {
        a_curr = 6 * a_prev1 - 9 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
