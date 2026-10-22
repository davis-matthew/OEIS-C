/*
 * A380830  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-3*x) / (1 +
 *          x) ).
 * https://oeis.org/A380830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (3*n+3)^k * binomial(n,k)/(k+1)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n! : factorial of n */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    /* B = 3*n+3 */
    seq_t B = (seq_t)(3 * n + 3);

    /* a_0 = fact */
    seq_t term = fact;
    seq_t sum  = term;

    /* iterate for k = 0 .. n-1 */
/*  a_{k+1} = a_k * B * (n-k) / ((k+1)*(k+2))  */
    for (idx_t k = 0; k < n; ++k)
    {
        term = term * B * (seq_t)(n - k);
        term = term / (seq_t)(k + 1);
        term = term / (seq_t)(k + 2);
        sum += term;
    }
    return sum;
}
