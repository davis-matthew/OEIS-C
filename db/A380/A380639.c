/*
 * A380639  Expansion of e.g.f. exp(x/(1 - 2*x)^2).
 * https://oeis.org/A380639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} 2^k * binomial(2*n-k-1,k)/(n-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t perm = 1;   /* P(n,0) = product of first 0 numbers = 1          */
    seq_t pow2 = 1;   /* 2^0 */
    seq_t T = 1;      /* binomial(2*n-0-1,0) = 1 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = 2^k * binomial(2*n-k-1,k) * P(n,k) */
        sum += pow2 * T * perm;

        if (k == n)
            break;                     /* finished */

        /* update P(n,k+1) = P(n,k) * (n - k) */
        perm *= (seq_t)(n - k);

        /* compute next binomial coefficient */
        idx_t kk = k + 1;
        seq_t num = T * (seq_t)(2 * n - 2 * kk + 1) *
                    (seq_t)(2 * n - 2 * kk);
        seq_t den = (seq_t)kk * (seq_t)(2 * n - kk);
        T = num / den;                 /* exact division */

        /* next power of two: 2^(k+1) */
        pow2 <<= 1;
    }

    return sum;
}
