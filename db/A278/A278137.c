/*
 * A278137  Maximum number of disjoint subgraphs of the Fibonacci cube
 *          Gamma(n) that are isomorphic to the hypercube of dimension k,
 *          summed over all k.
 * https://oeis.org/A278137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + 2*F(n); a(0)=1, a(1)=3, a(2)=4
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small cases directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;
    if (n == 2)
        return (seq_t)4;

    /* Fibonacci numbers: F(1)=1, F(2)=1   (F(0) not needed on first loop) */
    seq_t f_prevprev = (seq_t)1;   /* F(i-2) */
    seq_t f_prev     = (seq_t)1;   /* F(i-1) */
    seq_t f_cur;                   /* F(i) */

    /* Sliding window for a:  a(i-3), a(i-2), a(i-1) */
    seq_t a_minus3 = (seq_t)1;      /* a(0) */
    seq_t a_minus2 = (seq_t)3;      /* a(1) */
    seq_t a_prev   = (seq_t)4;      /* a(2) */
    seq_t curr;                    /* a(i) */

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute Fibonacci F(i) */
        f_cur = f_prevprev + f_prev;
        f_prevprev = f_prev;
        f_prev = f_cur;

        /* Compute a(i) using the recurrence */
        curr = a_minus2 + a_minus3 + f_cur + f_cur;   /* +2*F(i) */

        /* Shift the window for the next step */
        a_minus3 = a_minus2;
        a_minus2 = a_prev;
        a_prev   = curr;
    }

    return a_prev;   /* a(n) is now in a_prev */
}
