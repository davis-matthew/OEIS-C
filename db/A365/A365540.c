/*
 * A365540  Antidiagonal sums of A365539.
 * https://oeis.org/A365540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n} (Sum_{k=1..m} (2*A000005(k) - 1))
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t S1 = 0;   /* Σ_{k=1..n} d(k) */
    seq_t S2 = 0;   /* Σ_{k=1..n} k*d(k) */

    for (idx_t i = 1; i <= n; ) {
        idx_t q     = n / i;          /* floor(n / i) */
        idx_t last  = n / q;          /* largest j with floor(n/j) == q */
        seq_t cnt   = (seq_t)(last - i + 1);

        S1 += cnt * (seq_t)q;         /* Σ d(k) for k in [i,last] */

        /* Σ_{d=i}^{last} d = (i+last)*(last-i+1)/2 */
        seq_t sum_d = ((seq_t)i + (seq_t)last) * cnt / 2;

        /* contribution to Σ k*d(k) from d in [i,last] :
           d * q*(q+1)/2 for each d, summed over the interval */
        seq_t term = sum_d * (seq_t)q * ((seq_t)q + 1) / 2;
        S2 += term;

        i = last + 1;
    }

    seq_t n_seq = (seq_t)n;
    /* a(n) = 2 * ((n+1)*S1 - S2) - n*(n+1)/2 */
    seq_t result = 2 * ((n_seq + 1) * S1 - S2) - n_seq * (n_seq + 1) / 2;
    return result;
}
