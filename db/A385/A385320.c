/*
 * A385320  a(n) = Sum_{k=0..n} 2^k * binomial(3*n,k) * binomial(3*n-k-1,n-k).
 * https://oeis.org/A385320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(3*n,k) *
 *            binomial(3*n-k-1,n-k) Equivalently a(n) = Sum_{k=0..n} 3^k *
 *            (-1)^{n-k} * binom(3*n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Accumulate sums of terms with positive and negative sign
       according to the parity of k relative to n. */
    seq_t pos = 0, neg = 0;           /* sums of positive‑ and negative‑terms      */
    seq_t C   = 1;                    /* C = binom(3*n, k)                        */
    seq_t pow3k = 1;                  /* pow3k = 3^k                              */
    idx_t parity = n & 1UL;           /* n%2                                       */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = C * pow3k;

        /* sign = (-1)^(n-k) is positive when k%2 == parity */
        if ((k & 1UL) == parity)
            pos += term;
        else
            neg += term;

        if (k == n) break;            /* avoid an update after the last term */

        /* Update binomial C for next k */
        C = C * (seq_t)(3 * n - k);   /* multiply by (3n - k)                     */
        C = C / (k + 1);              /* divide by (k + 1)                        */

        /* Update 3^k */
        pow3k = pow3k * 3;
    }
    return pos - neg;                 /* pos contains all positive‑sign terms     */
}
