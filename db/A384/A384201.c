/*
 * A384201  Expansion of e.g.f. -log(1 - 2*x)/(2 * (1 + x)).
 * https://oeis.org/A384201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*a(n-1) + 2*(n-1)^2*a(n-2) with a(0) = 0, a(1) = 1.
 *            This is derived from the e.g.f. -log(1-2x)/(2(1+x))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A384201 */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) {
        return (seq_t)0;
    }
    if (n == 1) {
        return (seq_t)1;
    }

    /* Recurrence uses the two previous values */
    seq_t a_prev2 = (seq_t)0;   /* a(0) */
    seq_t a_prev1 = (seq_t)1;   /* a(1) */
    seq_t a_curr  = (seq_t)0;

    for (idx_t k = 2; k <= n; ++k) {
        seq_t term1 = (seq_t)(k - 2) * a_prev1;

        seq_t tmp   = (seq_t)(k - 1);          /* (k-1) */
        seq_t term2 = (seq_t)2 * tmp * tmp * a_prev2;

        a_curr = term1 + term2;

        /* shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
