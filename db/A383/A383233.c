/*
 * A383233  Expansion of e.g.f. f(x)^3 * log(f(x)), where f(x) = 1/(1 -
 *          5*x)^(1/5).
 * https://oeis.org/A383233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, for n>1 a(n) = (10*n-9)*a(n-1) - (5*n-7)^2 *
 *            a(n-2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    seq_t prev2 = 0;   /* a(n-2)   */
    seq_t prev1 = 1;   /* a(n-1)   */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t ti   = (seq_t)i;
        seq_t c1   = 10 * ti - 9;           /* 10*n-9   */
        seq_t c2   = 5  * ti - 7;           /* 5*n-7    */
        seq_t term1 = c1 * prev1;           /* (10*n-9) * a(n-1) */
        seq_t term2 = c2 * c2 * prev2;      /* (5*n-7)^2 * a(n-2) */
        seq_t cur   = term1 - term2;        /* a(n)     */

        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
