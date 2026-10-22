/*
 * A383281  a(n) = Sum_{k=0..n} (2*k+1) * (1/2)^(n+k) * (2*k)! * (n-k)! *
 *          binomial(n,k)^2.
 * https://oeis.org/A383281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A383281.
    a(0) = 1, a(1) = 2.
    For n > 1:
        a(n) = n * ( (n+1)*a(n-1) - ((n-1)^2)/2 * a(n-2) )
    The recurrence is rearranged to avoid fractions:
        a(n) = ( 2*n*(n+1)*a(n-1) - n*(n-1)*(n-1)*a(n-2) ) / 2
*/

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)2;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* term1 = 2 * i * (i+1) * prev1 */
        seq_t term1 = (seq_t)2 * (seq_t)i * (seq_t)(i + 1) * prev1;
        /* term2 = i * (i-1) * (i-1) * prev2 */
        seq_t term2 = (seq_t)i * (seq_t)(i - 1) * (seq_t)(i - 1) * prev2;
        /* numerator = term1 - term2, guaranteed even */
        seq_t num = term1 - term2;
        /* compute a(i) */
        seq_t curr = num / (seq_t)2;
        /* shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
