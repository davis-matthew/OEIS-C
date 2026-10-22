/*
 * A322628  Number of n-digit decimal numbers containing a fixed 2-digit
 *          integer with distinct digits as a substring.
 * https://oeis.org/A322628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-1) - a(n-2) + 9*10^(n-3)
   with a(0) = 0, a(1) = 0, a(2) = 1 */

seq_t a(idx_t n)
{
    /* Handle trivial cases explicitly */
    if (n == 0 || n == 1) return (seq_t)0;
    if (n == 2)           return (seq_t)1;

    /* a[1] = 0, a[2] = 1  (starting values for the loop) */
    seq_t a_prevprev = (seq_t)0;   /* a(i-2) */
    seq_t a_prev     = (seq_t)1;   /* a(i-1) */
    seq_t pow10      = (seq_t)1;   /* 10^(i-3), starts with i = 3 */

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute a(i) from the recurrence */
        seq_t term = a_prev * (seq_t)10
                     - a_prevprev
                     + (seq_t)9 * pow10;

        /* Advance the sliding window */
        a_prevprev = a_prev;
        a_prev     = term;

        /* Update power of 10 for the next iteration */
        pow10 *= (seq_t)10;
    }

    return a_prev;
}
