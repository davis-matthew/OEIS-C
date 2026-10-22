/*
 * A388726  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(3*n+3*k,k).
 * https://oeis.org/A388726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(3*n+3*k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t Cnk = 1;                     /* C(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute C(3*n+3*k, k) multiplicatively */
        seq_t Dk = 1;
        for (idx_t i = 1; i <= k; ++i) {
            /* num = 3*n + 2*k + i (all cast to seq_t to avoid overflow) */
            seq_t num = (seq_t)3 * (seq_t)n + (seq_t)2 * (seq_t)k + (seq_t)i;
            Dk = (Dk * num) / (seq_t)i;
        }

        /* Add the current term */
        sum += Cnk * Dk;

        /* Update C(n,k+1) for next iteration */
        if (k < n) {
            Cnk = (Cnk * (seq_t)(n - k)) / (seq_t)(k + 1);
        }
    }
    return sum;
}
