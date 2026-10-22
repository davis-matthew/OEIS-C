/*
 * A331794  a(n) = Sum_{k=0..n} n^k * binomial(n+1,k) * binomial(n+1,k+1).
 * https://oeis.org/A331794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} n^k * binomial(n+1,k) * binomial(n+1,k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t result = 0;
    seq_t B = 1;                   /* binomial(n+1, 0)           */
    seq_t B_next = (seq_t)(n + 1);  /* binomial(n+1, 1)           */
    seq_t power = 1;               /* n^0                        */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = n^k * C(n+1,k) * C(n+1,k+1) */
        seq_t term = power * B * B_next;
        result += term;

        if (k == n) break;          /* last iteration finished */

        /* update power of n */
        power *= (seq_t)n;

        /* update binomial coefficients:
           C(n+1,k+2) = C(n+1,k+1) * (n-k) / (k+2) */
        seq_t B_next_next = B_next * (seq_t)(n - k) / (seq_t)(k + 2);

        /* shift for next k */
        B = B_next;
        B_next = B_next_next;
    }

    return result;
}
