/*
 * A388053  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n^2+k+1,n).
 * https://oeis.org/A388053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A388053
 * Formula: a(n) = Sum_{k=0..n} 2^k * binomial(n,k) * binomial(n^2+1,k)
 * (equivalent to the OEIS definition)
 */

seq_t a(idx_t n)
{
    /* n^2 + 1 */
    seq_t N = (seq_t)n * (seq_t)n + 1;

    seq_t C_n = 1;          /* binomial(n,0) */
    seq_t C_N = 1;          /* binomial(N,0) */
    seq_t pow2 = 1;         /* 2^0 */
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Add current term */
        sum += pow2 * C_n * C_N;

        /* Prepare for next k */
        if (k < n) {
            /* C(n,k+1) = C(n,k) * (n-k) / (k+1) */
            C_n = C_n * ((seq_t)n - (seq_t)k) / ((seq_t)k + 1);

            /* C(N,k+1) = C(N,k) * (N-k) / (k+1) */
            C_N = C_N * (N - (seq_t)k) / ((seq_t)k + 1);

            /* pow2 = 2^(k+1) */
            pow2 = pow2 * 2;
        }
    }

    return sum;
}
