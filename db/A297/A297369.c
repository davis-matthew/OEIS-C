/*
 * A297369  Number of n X 3 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 0, 1 or 2 neighboring 1s.
 * https://oeis.org/A297369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 4*a(n-3) Initial values: a(1) = 8, a(2) = 57,
 *            a(3) = 393
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)57;
    if (n == 3) return (seq_t)393;

    /* Use a linear recurrence: a(n) = 7*a(n-1) - 4*a(n-3) */
    seq_t a1 = (seq_t)8;    /* a(n-3) */
    seq_t a2 = (seq_t)57;   /* a(n-2) */
    seq_t a3 = (seq_t)393;  /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)7 * a3 - (seq_t)4 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
