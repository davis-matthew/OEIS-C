/*
 * A266579  Numerator of the continued fraction [Sum_{k=0..n} k; 1, 2, 3,...,
 *          n].
 * https://oeis.org/A266579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Numerator of the continued fraction [n(n+1)/2; 1,2,…,n]
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n<1 */
    seq_t S = (seq_t)n * ((seq_t)n + 1) / 2;   /* triangular number T(n) */
    seq_t p_prev2 = 0;                     /* p_{-2} */
    seq_t p_prev1 = 1;                     /* p_{-1} */
    seq_t cur = S;                         /* p_0 = S */

    /* shift to state after the first term */
    p_prev2 = p_prev1;
    p_prev1 = cur;

    /* iterate through the terms 1,2,…,n. */
    for (idx_t i = 1; i <= n; ++i)
    {
        cur = (seq_t)i * p_prev1 + p_prev2; /* p_i = i*p_{i-1} + p_{i-2} */
        p_prev2 = p_prev1;
        p_prev1 = cur;
    }
    return cur;
}
