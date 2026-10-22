/*
 * A386845  a(n) = Sum_{k=0..n} binomial(4*n+2,k) * binomial(4*n-k,n-k).
 * https://oeis.org/A386845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n+2,k) * binomial(4*n-k,n-k)
 *            Computed using the equivalent form a(n) = Sum_{k=0..n}
 *            2^k*(n-k+1)*binomial(3*n+k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow2   = 1; /* current value of 2^k   */
    seq_t C      = 1; /* current binomial(3n+k, k) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = 2^k * (n-k+1) * binomial(3n+k, k) */
        seq_t term = pow2;
        term *= (seq_t)(n - k + 1);
        term *= C;
        result += term;

        /* prepare for next k: 2^k -> 2^{k+1}, binomial increment */
        if (k == n) break;         /* no next term needed */

        pow2 *= 2;

        /* C_{k+1} = C_k * (3n + k + 1) / (k + 1) */
        C *= (seq_t)(3 * n + k + 1);
        C /= (seq_t)(k + 1);
    }
    return result;
}
