/*
 * A350917  a(0) = 1, a(1) = 2, and a(n) = 23*a(n-1) - a(n-2) - 4 for n >= 2.
 * https://oeis.org/A350917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2, a(n) = 23*a(n-1) - a(n-2) - 4 for n >= 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)2;   /* a(1) */
    seq_t a_curr  = 0;

    /* Iterate from 2 to n using the recurrence */
    for (idx_t i = 2; i <= n; ++i) {
        a_curr = ((seq_t)23 * a_prev1) - a_prev2 - (seq_t)4;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
