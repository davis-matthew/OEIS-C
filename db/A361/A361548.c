/*
 * A361548  Expansion of e.g.f. exp((x + x^2/2 + x^3/6)/(1-x)).
 * https://oeis.org/A361548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1)*a(n-1) - (n-1)*(n-3)*a(n-2) -
 *            2*binomial(n-1,3)*a(n-4) with a(0)=1, a(1)=1, a(2)=4, a(3)=20
 *            binomial(n-1,3) = (n-1)*(n-2)*(n-3)/6 The function computes the
 *            value for any n ≥ 0 using only the recurrence. It is
 *            order‑independent and works for arbitrary-precision or
 *            fixed‑width seq_t...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)20;

    /* a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a_m4 = (seq_t)1;      /* a(0)          */
    seq_t a_m3 = (seq_t)1;      /* a(1)          */
    seq_t a_m2 = (seq_t)4;      /* a(2)          */
    seq_t a_m1 = (seq_t)20;     /* a(3)          */
    seq_t a_n  = 0;             /* current a(n)  */

    for (idx_t i = 4; i <= n; ++i) {
        /* term1 = (2*i-1) * a(n-1) */
        seq_t term1 = ((seq_t)(2*i - 1)) * a_m1;

        /* term2 = (i-1)*(i-3) * a(n-2) */
        seq_t term2 = ((seq_t)(i-1) * (i-3)) * a_m2;

        /* bin = (i-1)*(i-2)*(i-3) / 6  */
        seq_t bin = ((seq_t)(i-1) * (i-2) * (i-3)) / 6;

        /* term3 = 2 * bin * a(n-4) */
        seq_t term3 = ((seq_t)2 * bin) * a_m4;

        a_n = term1 - term2 - term3;

        /* shift for next iteration */
        a_m4 = a_m3;
        a_m3 = a_m2;
        a_m2 = a_m1;
        a_m1 = a_n;
    }

    return a_n;
}
