/*
 * A370097  a(n) = Sum_{k=0..n} binomial(3*n,k) * binomial(3*n-k-1,n-k).
 * https://oeis.org/A370097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*n,k) * binomial(3*n-k-1,n-k)
 *            Equivalent convenient form used in the code: a(n) = Sum_{k=0..n}
 *            2^k * binomial(2*n + k - 1, k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return 1;

    seq_t sum    = 0;          /* running total */
    seq_t binom  = 1;          /* binomial(2*n + k - 1, k) for current k */
    seq_t pow2k  = 1;          /* 2^k for current k */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* add current term */
        sum += binom * pow2k;

        if (k == n)
            break;             /* last term processed */

        /* update binomial for next k: binom_{k+1} = binom_k * (2*n + k) / (k+1) */
        binom = (binom * (seq_t)(2 * n + k)) / (seq_t)(k + 1);

        /* double the power of two */
        pow2k *= 2;
    }

    return sum;
}
