/*
 * A385633  a(n) = a(n-1) + a(n-3), with a(0) = 1, a(1) = 4, a(2) = 8.
 * https://oeis.org/A385633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3), a(0) = 1, a(1) = 4, a(2) = 8
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;

    /* Iteratively compute using the recurrence. */
    seq_t a0 = (seq_t)1;   /* a(i-3) */
    seq_t a1 = (seq_t)4;   /* a(i-2) */
    seq_t a2 = (seq_t)8;   /* a(i-1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = a2 + a0;   /* a(i) = a(i-1) + a(i-3) */
        a0 = a1;                /* shift window */
        a1 = a2;
        a2 = next;
    }
    return a2;
}
