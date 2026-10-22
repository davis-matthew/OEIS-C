/*
 * A308876  Expansion of e.g.f. exp(x)*(1 - x)/(1 - 2*x).
 * https://oeis.org/A308876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n} binomial(n,k) * 2^(k-1) * k!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                     /* a(0) = 1 */
        return (seq_t)1;

    seq_t t = (seq_t)n;              /* term for k = 1  : binomial(n,1)*2^0*1! = n */
    seq_t sum = t;                   /* accumulate terms  */

    for (idx_t k = 2; k <= n; ++k) {
        /* t_k = t_{k-1} * 2 * (n - k + 1)  (derived from the recurrence) */
        idx_t factor = 2 * (n - k + 1);
        t *= (seq_t)factor;
        sum += t;
    }

    return sum + (seq_t)1;           /* add the leading 1 */
}
