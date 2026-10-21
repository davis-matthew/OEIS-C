/*
 * A265380  Binary representation of the middle column of the "Rule 158"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A265380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-4) - 10*a(n-5) for n > 4 Initial terms:
 *            a(0)=1, a(1)=11, a(2)=111, a(3)=1110, a(4)=11101
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)111;
    if (n == 3) return (seq_t)1110;
    if (n == 4) return (seq_t)11101;

    /* Keep the five most recent terms: a(n-5), a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a_prev5 = (seq_t)1;      /* a(0)   */
    seq_t a_prev4 = (seq_t)11;     /* a(1)   */
    seq_t a_prev3 = (seq_t)111;    /* a(2)   */
    seq_t a_prev2 = (seq_t)1110;   /* a(3)   */
    seq_t a_prev1 = (seq_t)11101;  /* a(4)   */
    seq_t a_curr = 0;

    for (idx_t i = 5; i <= n; ++i)
    {
        /* a(i) = 10*a(i-1) + a(i-4) - 10*a(i-5) */
        a_curr = 10*a_prev1 + a_prev4 - 10*a_prev5;

        /* shift the window */
        a_prev5 = a_prev4;
        a_prev4 = a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
