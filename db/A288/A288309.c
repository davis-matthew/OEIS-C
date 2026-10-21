/*
 * A288309  a(n) = 2*a(n-1) + 2*a(n-2) - 3*a(n-3), where a(0) = 2, a(1) = 4,
 *          a(2) = 8.
 * https://oeis.org/A288309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 3*a(n-3), a(0) = 2, a(1) = 4, a(2)
 *            = 8
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the three fixed initial terms */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;

    /* Working variables to hold the recurrence states.
       a0 = a(i-3), a1 = a(i-2), a2 = a(i-1) for the current i. */
    seq_t a0 = (seq_t)2;
    seq_t a1 = (seq_t)4;
    seq_t a2 = (seq_t)8;
    seq_t ai = 0;   /* will hold a(i) during the loop */

    for (idx_t i = 3; i <= n; ++i) {
        ai = (seq_t)2 * a2 + (seq_t)2 * a1 - (seq_t)3 * a0;
        a0 = a1;   /* shift the window for the next iteration */
        a1 = a2;
        a2 = ai;
    }

    return ai;
}
