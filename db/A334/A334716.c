/*
 * A334716  a(n) = !n + n * n!, where !n = A000166(n) is subfactorial of n.
 * https://oeis.org/A334716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = !n + n * n! (OEIS A334716)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0) return (seq_t)1;            /* !0 + 0·0! = 1 */
    if (n == 1) return (seq_t)1;            /* !1 + 1·1! = 0 + 1 = 1 */

    seq_t fact   = 1;        /* n!  */
    seq_t sub0   = 1;        /* !0  */
    seq_t sub1   = 0;        /* !1  */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t temp = sub1 + sub0;              /* !(i-1) + !(i-2) */
        sub0 = sub1;                            /* shift */
        sub1 = (seq_t)(i - 1) * temp;           /* !i = (i-1)·( !(i-1)+!(i-2) ) */
        fact *= i;                             /* accumulate n! */
    }

    return sub1 + (seq_t)n * fact;              /* !n + n·n! */
}
