/*
 * A294190  Expansion of e.g.f. exp(3*(1/(1-x)^3 - 1)).
 * https://oeis.org/A294190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n+5)*a(n-1) - 6*(n-2)*(n-1)*a(n-2) * +
 *            4*(n-3)*(n-2)*(n-1)*a(n-3) - (n-4)*(n-3)*(n-2)*(n-1)*a(n-4), *
 *            with a(0)=1, a(1)=9, a(2)=117, a(3)=1881
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases. The cast to seq_t is optional but keeps the intent clear. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)117;
    if (n == 3) return (seq_t)1881;

    /* Previous four values of the sequence. */
    seq_t t0 = (seq_t)1;       /* a(0)   */
    seq_t t1 = (seq_t)9;       /* a(1)   */
    seq_t t2 = (seq_t)117;     /* a(2)   */
    seq_t t3 = (seq_t)1881;    /* a(3)   */

    seq_t tn;
    for (idx_t i = 4; i <= n; ++i) {
        /* Coefficients computed in seq_t to avoid truncation. */
        seq_t c1 = (seq_t)4 * (seq_t)i + (seq_t)5;
        seq_t c2 = (seq_t)6 * (seq_t)(i - 2) * (seq_t)(i - 1);
        seq_t c3 = (seq_t)4 * (seq_t)(i - 3) * (seq_t)(i - 2) * (seq_t)(i - 1);
        seq_t c4 = (seq_t)(i - 4) * (seq_t)(i - 3) * (seq_t)(i - 2) * (seq_t)(i - 1);

        tn  = c1 * t3;
        tn -= c2 * t2;
        tn += c3 * t1;
        tn -= c4 * t0;

        /* Shift the window of previous values for next iteration. */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = tn;
    }

    return t3; /* t3 holds a(n) after the loop. */
}
