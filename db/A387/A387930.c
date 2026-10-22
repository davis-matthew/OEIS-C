/*
 * A387930  a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(3*n,k).
 * https://oeis.org/A387930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(3*n,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = (seq_t)1;   /* term for k=0 is 1 */
    seq_t term  = (seq_t)1;   /* current term   */
    seq_t nn    = (seq_t)n;   /* n as seq_t    */
    seq_t t3n   = (seq_t)3 * nn; /* 3*n as seq_t */

    /* iterate k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* term = term * (n - k + 1) / k * (3*n - k + 1) / k * 3  */
        term = term * (nn - (seq_t)k + (seq_t)1);   /* multiply by (n-k+1) */
        term = term / (seq_t)k;                     /* divide by k */
        term = term * (t3n - (seq_t)k + (seq_t)1);  /* multiply by (3n-k+1) */
        term = term / (seq_t)k;                     /* divide by k */
        term = term * (seq_t)3;                     /* multiply by 3^1 */

        sum = sum + term;              /* add current term to sum */
    }

    return sum;
}
