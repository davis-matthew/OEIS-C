/*
 * A383231  Expansion of e.g.f. f(x) * log(f(x)), where f(x) = 1/(1 -
 *          5*x)^(1/5).
 * https://oeis.org/A383231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(n) = (10n-13)*a(n-1) - (5n-9)^2*a(n-2) for n ≥ 2 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)0;   /* a(n-2) */
    seq_t a1 = (seq_t)1;   /* a(n-1) */
    seq_t an = a1;         /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t factor1 = (seq_t)10 * (seq_t)i - (seq_t)13;          /* 10*i-13 */
        seq_t term1   = factor1 * a1;                              /* (10*i-13)*a(i-1) */

        seq_t f       = (seq_t)5 * (seq_t)i - (seq_t)9;            /* 5*i-9 */
        seq_t term2   = f * f * a0;                                 /* (5*i-9)^2*a(i-2) */

        an = term1 - term2;                                        /* a(i) */

        a0 = a1;                                                   /* shift window */
        a1 = an;
    }
    return an;
}
