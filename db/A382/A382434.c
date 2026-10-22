/*
 * A382434  a(n) = Sum_{k=0..n} ( binomial(n,k) - binomial(n,k-1) )^4.
 * https://oeis.org/A382434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} ( binomial(n,k) - binomial(n,k-1) )^4
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* This implementation evaluates the sum directly.  The difference
       binomial(n,k) - binomial(n,k-1) can be negative for k larger
       than n/2, so we take its absolute value before raising it to the
       fourth power.  All intermediate results are computed using the
       operations available for seq_t. */
    seq_t sum = 0;            /* running total of the sum */
    seq_t prev = 0;           /* binomial(n,k-1)  (0 for k = 0)   */
    seq_t cur  = 1;           /* binomial(n,0)    */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* absolute difference |C(n,k) - C(n,k-1)| */
        seq_t diff = (cur >= prev) ? (cur - prev) : (prev - cur);

        /* diff^4 = (diff*diff)^2 */
        seq_t diff2 = diff * diff;
        seq_t diff4 = diff2 * diff2;

        sum += diff4;

        /* prepare for next k */
        prev = cur;
        if (k < n)
        {
            /* C(n,k+1) = C(n,k) * (n-k) / (k+1)   */
            seq_t mult = cur * (seq_t)(n - k);
            cur = mult / (seq_t)(k + 1);
        }
    }
    return sum;
}
