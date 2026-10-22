/*
 * A347012  E.g.f.: exp(x) / (1 - 4 * x)^(1/4).
 * https://oeis.org/A347012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} binomial(n,k) * prod_{t=0}^{k-1} (4t+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 0;          /* running sum of the terms            */
    seq_t binom = 1;          /* current binomial coefficient C(n,k) */
    seq_t P     = 1;          /* product prod_{t=0}^{k-1} (4t+1)     */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = binom(n,k) * P(k) */
        seq_t term = binom * P;
        sum = sum + term;

        if (k == n)            /* last iteration – no more updates needed */
            break;

        /* update binomial coefficient */
        binom = binom * (n - k);   /* multiply by (n-k)      */
        binom = binom / (k + 1);   /* then divide by (k+1)   */

        /* update product P(k) -> P(k+1) */
        P = P * (4 * k + 1);
    }

    return sum;
}
