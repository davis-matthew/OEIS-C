/*
 * A387928  a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(2*n,k).
 * https://oeis.org/A387928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(2*n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t c1  = 1;          /* C(n, 0)   */
    seq_t c2  = 1;          /* C(2n, 0)  */
    seq_t pow3 = 1;         /* 3^0       */
    seq_t two_n = (seq_t)(2 * n);

    for (idx_t k = 0; k <= n; ++k)        /* iterate k = 0 .. n */
    {
        /* term = c1 * c2 * pow3 */
        sum += c1 * c2 * pow3;

        if (k == n) break;               /* last term processed */

        /* Update binomial coefficients for next k */
        seq_t nk    = (seq_t)(n - k);
        seq_t denom = (seq_t)(k + 1);

        c1 = (c1 * nk) / denom;                        /* C(n, k+1) */
        seq_t two_n_k = two_n - (seq_t)k;
        c2 = (c2 * two_n_k) / denom;                  /* C(2n, k+1) */

        pow3 = pow3 * (seq_t)3;                       /* 3^(k+1)   */
    }
    return sum;
}
