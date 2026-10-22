/*
 * A324591  E.g.f.: exp(2 * (x + x^2 / 2 + x^3 / 3)).
 * https://oeis.org/A324591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (a(n-1) + (n-1) * a(n-2) + (n-1) * (n-2) * a(n-3)),
 *            with initial values a(0)=1, a(1)=2, a(2)=6
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;
    if (n == (idx_t)2) return (seq_t)6;

    seq_t a0 = (seq_t)1;        /* a(n-3)  */
    seq_t a1 = (seq_t)2;        /* a(n-2)  */
    seq_t a2 = (seq_t)6;        /* a(n-1)  */
    seq_t ai;

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        seq_t term = a2;                            /* a(n-1)                  */
        term += (seq_t)(i-1) * a1;                  /* (n-1) * a(n-2)          */
        term += (seq_t)(i-1) * (i-2) * a0;          /* (n-1)(n-2) * a(n-3)     */
        ai = (seq_t)2 * term;                       /* 2 * term                 */

        /* shift window for next iteration   */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
