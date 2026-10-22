/*
 * A321837  Expansion of e.g.f.: exp(x/(1-3*x)).
 * https://oeis.org/A321837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*n-5)*a(n-1) - 9*(n-2)*(n-1)*a(n-2) , a(0)=1, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* a(0) and a(1) */
    seq_t a_prev2 = (seq_t)1;  /* a(0) */
    seq_t a_prev1 = (seq_t)1;  /* a(1) */
    seq_t an;

    /* iterate from 2 up to n using the recurrence */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term1 = (seq_t)(6 * i - 5) * a_prev1;
        seq_t term2 = (seq_t)9 * (i - 2) * (i - 1) * a_prev2;
        an = term1 - term2;

        a_prev2 = a_prev1;
        a_prev1 = an;
    }

    return a_prev1;
}
