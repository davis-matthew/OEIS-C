/*
 * A391468  Number of words of length 3n formed with 3 types of letter where
 *          the number of each type == 0 (mod 3).
 * https://oeis.org/A391468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27 * (a(n-1) - a(n-2) + 27 * a(n-3)) a(0) = 1, a(1) = 3,
 *            a(2) = 63, a(3) = 2187
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)63;
    if (n == 3) return (seq_t)2187;

    /* Initialise the three most recent terms a(n-3), a(n-2), a(n-1). */
    seq_t prev3 = (seq_t)3;       /* a(1) */
    seq_t prev2 = (seq_t)63;      /* a(2) */
    seq_t prev1 = (seq_t)2187;    /* a(3) */
    seq_t curr;

    /* Compute forward using the recurrence. */
    for (idx_t i = 4; i <= n; ++i) {
        curr = (seq_t)27 * (prev1 - prev2 + (seq_t)27 * prev3);
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
