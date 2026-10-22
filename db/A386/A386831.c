/*
 * A386831  a(n) = Sum_{k=0..n} 3^k * 2^(n-k) * binomial(4*n+1,k) *
 *          binomial(4*n-k,n-k).
 * https://oeis.org/A386831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 5^k * (-2)^(n-k) * binomial(4*n+1,k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t pow2_n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2_n *= (seq_t)2;

    seq_t pow5k = 1;          /* 5^k          */
    seq_t pow2nk = pow2_n;    /* 2^(n-k) for current k */
    seq_t bin = 1;            /* C(4*n+1, k)  starting at k=0 */

    seq_t pos = 0, neg = 0;   /* sums of terms with + and - signs */

    idx_t N = 4 * n + 1;      /* constant 4*n+1 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* temp = C(4*n+1, k) * 2^(n-k) * 5^k */
        seq_t temp = bin * pow2nk;
        temp *= pow5k;

        /* sign: (-2)^(n-k) is negative when (n-k) is odd */
        if ((n - k) & 1)
            neg += temp;
        else
            pos += temp;

        if (k == n)
            break;  /* last iteration, nothing to update */

        /* update for next k */
        pow5k *= (seq_t)5;
        pow2nk /= (seq_t)2;

        /* binomial recurrence: C(N, k+1) = C(N, k) * (N-k) / (k+1) */
        seq_t numerator = (seq_t)(N - k);
        bin = bin * numerator;
        bin = bin / (seq_t)(k + 1);
    }

    return pos - neg;  /* pos >= neg for all n */
}
