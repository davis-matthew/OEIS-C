/*
 * A386868  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(2*n+2,k) *
 *          binomial(2*n-k,n-k).
 * https://oeis.org/A386868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * (n-k+1) * binomial(n+k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 0;          /* running sum of the terms                      */
    seq_t c     = 1;          /* binomial(n + k, k) for the current k          */
    seq_t pow3  = 1;          /* 3^k for the current k                         */
    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = 3^k * binomial(n+k,k) * (n-k+1) */
        seq_t term = pow3;
        term = term * c;
        term = term * ((seq_t)n - k + 1);
        sum += term;

        /* Prepare values for the next iteration (k+1) */
        if (k == n) break;                 /* avoid unnecessary update */

        pow3 = pow3 * (seq_t)3;            /* multiply by 3 for next k          */
        c = c * ((seq_t)(n + k + 1));      /* C(n+k+1, k+1) numerator part      */
        c = c / ((seq_t)(k + 1));          /* exact division for the combinatorial */
    }
    return sum;
}
