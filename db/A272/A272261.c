/*
 * A272261  Number of one-to-one functions f from [n] to [2n] where f(x) may
 *          not be equal to x or to 2n+1-x.
 * https://oeis.org/A272261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(1)=0; a(n) = 2*(2*n-1)*a(n-1) + 4*a(n-2) */
/* This recurrence is equivalent to the OEIS sum formula for A272261. */

seq_t a(idx_t n)
{
    /* Handle the two initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* a(0) and a(1) for the recurrence loop. */
    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)0;  /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* multiplier = 2 * (2*i - 1) */
        seq_t mi = (seq_t)2 * (((seq_t)2 * i) - (seq_t)1);
        /* recurrence: cur = mi * prev1 + 4 * prev2 */
        cur = mi * prev1 + (seq_t)4 * prev2;
        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
