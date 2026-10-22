/*
 * A341491  a(n) = binomial(6*n, n) * hypergeom([-5*n, -n], [-6*n], -1).
 * https://oeis.org/A341491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(n,k) * binomial(5*n,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin_n = 1;          /* binomial(n,k) for current k */
    seq_t bin_5n = 1;         /* binomial(5*n,k) for current k */
    seq_t pow2k = 1;          /* 2^k for current k */
    idx_t five_n = 5 * n;     /* 5*n as idx_t, safe for n ≤ 10000 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = bin_n * bin_5n * pow2k;
        sum += term;

        if (k < n) {
            /* Update binomial(n,k+1) from binomial(n,k) */
            bin_n = (bin_n * (seq_t)(n - k)) / (seq_t)(k + 1);

            /* Update binomial(5*n,k+1) from binomial(5*n,k) */
            bin_5n = (bin_5n * (seq_t)(five_n - k)) / (seq_t)(k + 1);

            /* 2^{k+1} from 2^k */
            pow2k = pow2k * 2;
        }
    }
    return sum;
}
