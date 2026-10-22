/*
 * A374675  a(n) = Sum_{k=0..n} binomial(n+k-1,n) * binomial(n+k,n).
 * https://oeis.org/A374675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k-1,n) * binomial(n+k,n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    /* b1 = C(n+k-1,n)   for current k
       b2 = C(n+k  ,n)   for current k   */
    seq_t b1 = 1;        /* for k = 1 : C(n,n) = 1   */
    seq_t b2 = (seq_t)n + 1;   /* for k = 1 : C(n+1,n) = n+1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        sum += b1 * b2;

        if (k == n)
            break;

        /* Update for next k:  b1_{k+1} = b1_k * (n+k) / k
                               b2_{k+1} = b2_k * (n+k+1) / (k+1) */
        b1 = b1 * (seq_t)(n + k) / k;
        b2 = b2 * (seq_t)(n + k + 1) / (k + 1);
    }

    return sum;
}
