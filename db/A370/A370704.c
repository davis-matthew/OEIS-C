/*
 * A370704  a(n) = Sum_{k=0..n} k!*binomial(n, k)*Pochhammer(n, k). Row sums
 *          of A370707.
 * https://oeis.org/A370704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k!*C(n,k)*Pochhammer(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence used: term_{k+1} = term_k * (n-k)*(n+k)  */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t sum  = (seq_t)1;   /* term_0 */
    seq_t term = (seq_t)1;   /* term_0 */
    for (idx_t k = 0; k < n; ++k) {
        /* Compute term_{k+1} = term_k * (n-k)*(n+k) */
        term = term * (seq_t)(n - k);
        term = term * (seq_t)(n + k);
        sum  = sum + term;
    }
    return sum;
}
