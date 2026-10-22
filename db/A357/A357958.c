/*
 * A357958  a(n) = 5*A005259(n) + 14*A005258(n-1).
 * https://oeis.org/A357958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*A005259(n) + 14*A005258(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum1 = (seq_t)0;        /* Σ_{k=0..n}   C(n,k)^2   * C(n+k,k)^2   */
    seq_t sum2 = (seq_t)0;        /* Σ_{k=0..n-1} C(n-1,k)^2 * C(n-1+k,k)   */

    seq_t c   = (seq_t)1;         /* C(n,0)        */
    seq_t d   = (seq_t)1;         /* C(n+0,0)      */

    seq_t c1  = (seq_t)1;         /* C(n-1,0)      */
    seq_t d1  = (seq_t)1;         /* C((n-1)+0,0)  */

    for (idx_t k = (idx_t)0; k <= n; ++k)           /* k runs 0..n          */
    {
        /* term for A005259 */
        seq_t cc  = c * c;          /* C(n,k)^2   */
        seq_t dd  = d * d;          /* C(n+k,k)^2 */
        sum1 += cc * dd;

        /* term for A005258 (only up to k = n-1) */
        if (k < n)
        {
            seq_t cc1 = c1 * c1;    /* C(n-1,k)^2 */
            sum2 += cc1 * d1;
        }

        /* update binomial factors for the next k (if any) */
        if (k < n)
        {
            /* C(n,k+1)   = C(n,k)   * (n-k)   / (k+1)   */
            /* C(n+k+1,k+1) = C(n+k,k) * (n+k+1) / (k+1) */
            /* C(n-1,k+1) = C(n-1,k) * ((n-1)-k) / (k+1) */
            /* C((n-1)+k+1,k+1) = C((n-1)+k,k) * ((n-1)+k+1) / (k+1) */

            seq_t denom = (seq_t)(k + (idx_t)1);

            c   = c   * (seq_t)(n - k)        / denom;
            d   = d   * (seq_t)(n + k + (idx_t)1) / denom;
            c1  = c1  * (seq_t)((n - (idx_t)1) - k) / denom;
            d1  = d1  * (seq_t)((n - (idx_t)1) + k + (idx_t)1) / denom;
        }
    }

    /* final combination */
    return sum1 * (seq_t)5 + sum2 * (seq_t)14;
}
