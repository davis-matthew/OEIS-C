/*
 * A335091  a(n) = ((2*n+1)!!)^3 * (Sum_{k=1..n} 1/(2*k+1)^3).
 * https://oeis.org/A335091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1 * a(n) = ((2*n-1)^3 + (2*n+1)^3) * a(n-1) -
 *            (2*n-1)^6 * a(n-2) for n>1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)  return 0;
    if (n == 1)  return 1;

    seq_t prev2 = 0;   /* a(n-2) */
    seq_t prev1 = 1;   /* a(n-1) */
    seq_t cur   = 0;   /* a(n)   */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* (2*i-1)^3 and (2*i+1)^3 */
        seq_t t1 = (seq_t)(2*i - 1);
        seq_t t1sq = t1 * t1;
        seq_t t1cubed = t1sq * t1;

        seq_t t2 = (seq_t)(2*i + 1);
        seq_t t2sq = t2 * t2;
        seq_t t2cubed = t2sq * t2;

        seq_t coef = t1cubed + t2cubed;   /* ((2i-1)^3 + (2i+1)^3) */

        /* (2*i-1)^6 */
        seq_t pow6 = 1;
        seq_t base = t1;                 /* 2i-1 */
        for (int k = 0; k < 6; ++k)
            pow6 *= base;

        /* recurrence */
        seq_t term1 = coef * prev1;
        seq_t term2 = pow6 * prev2;
        cur = term1 - term2;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
