/*
 * A353546  Expansion of e.g.f. -log(1-2*x) * exp(x)/2.
 * https://oeis.org/A353546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n) = (2*n-1)*a(n-1) - 2*(n-1)*a(n-2) + 1
 *            (for n>=2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)1;

    seq_t a_prev2 = (seq_t)0;  /* a(0) */
    seq_t a_prev1 = (seq_t)1;  /* a(1) */
    seq_t a_curr = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t coef1 = (seq_t)(2 * i - 1);          /* (2*i-1)   */
        seq_t coef2 = (seq_t)(2 * (i - 1));        /* 2*(i-1)   */

        a_curr = coef1 * a_prev1 - coef2 * a_prev2 + (seq_t)1;

        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
