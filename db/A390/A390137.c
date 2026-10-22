/*
 * A390137  Expansion of 1/((1-5*x) * sqrt(1+4*x)).
 * https://oeis.org/A390137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + (-1)^n * binomial(2*n,n), a(0) = 1, a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* a(n-1) for current iteration */
    seq_t a_prev = (seq_t)3;      /* a(1) */

    /* C(2,1) = 2  initial binomial value   */
    seq_t bin = (seq_t)2;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Update binomial coefficient C(2*i, i) from previous
         * C(2*(i-1), i-1) by the recurrence:
         * C(2i, i) = C(2i-2, i-1) * 2*(2*i-1)/i
         */
        seq_t mult = (seq_t)2 * ((seq_t)(2 * i - 1));
        bin = (bin * mult) / (seq_t)i;

        /* Compute a(i) = 5*a_prev + (-1)^i * bin */
        seq_t term = (seq_t)5 * a_prev;
        if ((i & 1) == 0)       /* even i: plus bin */
            term += bin;
        else                    /* odd i:  minus bin */
            term -= bin;

        a_prev = term;          /* move forward for next step */
    }

    return a_prev;
}
