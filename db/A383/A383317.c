/*
 * A383317  Expansion of e.g.f. exp(x/2) / (1-6*x)^(1/12).
 * https://oeis.org/A383317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*n-5)*a(n-1) - 3*(n-1)*a(n-2) with a(0)=1, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)1;  /* a(n-2) */
    seq_t a1 = (seq_t)1;  /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t coeff1 = (seq_t)(6 * i - 5);           /* (6*i-5) */
        seq_t coeff2 = (seq_t)(3 * (i - 1));        /* 3*(i-1)    */
        seq_t ai = coeff1 * a1 - coeff2 * a0;       /* a(i)       */
        a0 = a1;                                    /* shift window */
        a1 = ai;
    }
    return a1;
}
