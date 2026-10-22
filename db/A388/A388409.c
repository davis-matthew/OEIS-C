/*
 * A388409  a(n) = Sum_{k=0..n} 3^(n-k) * binomial(n,k) * binomial(n+2,k).
 * https://oeis.org/A388409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^(n-k) * binomial(n,k) * binomial(n+2,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 *= (seq_t)3;

    seq_t binom_n  = 1;   /* binomial(n,0)   */
    seq_t binom_n2 = 1;   /* binomial(n+2,0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        sum += binom_n * binom_n2 * pow3;
        if (k == n)
            break;
        /* update binomial(n,k+1) */
        binom_n  = (binom_n  * (seq_t)(n - k))  / (seq_t)(k + 1);
        /* update binomial(n+2,k+1) */
        binom_n2 = (binom_n2 * (seq_t)(n + 2 - k)) / (seq_t)(k + 1);
        /* next power of 3 */
        pow3 /= (seq_t)3;
    }

    return sum;
}
