/*
 * A303209  Number of total dominating sets in the n X n rook complement
 *          graph.
 * https://oeis.org/A303209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n^2) - 2*n*(2^n - 1) - 2*n^2*(2^(n-1)-1)^2 +
 *            n^2*(n-1)^2/2 + n^2 - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2_seq(idx_t exp)
{
    seq_t res  = 1;
    seq_t base = 2;
    while (exp) {
        if (exp & 1)  res  = res * base;
        base = base * base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t N      = (seq_t)n;
    seq_t N2     = N * N;
    seq_t pow2_nsq   = pow2_seq((idx_t)(n * n));
    seq_t pow2_n     = pow2_seq((idx_t)n);
    seq_t pow2_nm1   = pow2_seq((idx_t)(n - 1));

    seq_t term1 = pow2_nsq;
    seq_t term2 = 2 * N * (pow2_n - 1);

    seq_t t_sub = pow2_nm1 - 1;
    seq_t term3 = 2 * N2 * (t_sub * t_sub);

    seq_t term4 = N2 * (N - 1) * (N - 1) / 2;
    seq_t term5 = N2 - 1;

    return term1 - term2 - term3 + term4 + term5;
}
