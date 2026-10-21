/*
 * A289795  p-INVERT of (3n), where p(S) = 1 - S - S^2.
 * https://oeis.org/A289795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 3*a(n-2) + 7*a(n-3) - a(n-4); Initial values:
 *            a(0) = 3, a(1) = 24, a(2) = 162, a(3) = 1083
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)162;
    if (n == 3) return (seq_t)1083;

    /* Use four successive values to iterate the recurrence. */
    seq_t a0 = (seq_t)3;      /* a(n-4)   */
    seq_t a1 = (seq_t)24;     /* a(n-3)   */
    seq_t a2 = (seq_t)162;    /* a(n-2)   */
    seq_t a3 = (seq_t)1083;   /* a(n-1)   */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 7*a(i-1) - 3*a(i-2) + 7*a(i-3) - a(i-4) */
        next = (seq_t)7 * a3 - (seq_t)3 * a2 + (seq_t)7 * a1 - a0;

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;   /* a(n) is in a3 after the loop */
}
