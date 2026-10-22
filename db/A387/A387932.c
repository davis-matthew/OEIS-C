/*
 * A387932  a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(4*n,k).
 * https://oeis.org/A387932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(4*n,k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin_n = 1;      /* binomial(n,0) */
    seq_t bin_4n = 1;     /* binomial(4*n,0) */
    seq_t pow3 = 1;       /* 3^0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* Current term of the sum */
        seq_t term = bin_n * bin_4n * pow3;
        sum += term;

        if (k == n)
            break;

        idx_t kp = k + 1;

        /* Update binomial(n,k+1) from binomial(n,k) */
        bin_n = bin_n * (seq_t)(n - k) / (seq_t)kp;

        /* Update binomial(4*n,k+1) */
        bin_4n = bin_4n * (seq_t)(4 * n - k) / (seq_t)kp;

        /* Update 3^k to 3^(k+1) */
        pow3 = pow3 * (seq_t)3;
    }

    return sum;
}
