/*
 * A290907  a(n) = (1/3)*A290906(n).
 * https://oeis.org/A290907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 3*a(n-2) + 4*a(n-3) - a(n-4)
   Initial values: a(0)=0, a(1)=1, a(2)=4, a(3)=13 */
seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)13;

    /* iterative computation for n >= 4 */
    seq_t a0 = (seq_t)0;   /* a[n-4] */
    seq_t a1 = (seq_t)1;   /* a[n-3] */
    seq_t a2 = (seq_t)4;   /* a[n-2] */
    seq_t a3 = (seq_t)13;  /* a[n-1] */
    seq_t next = 0;
    idx_t i;

    for (i = 4; i <= n; ++i) {
        next = 4 * a3 - 3 * a2 + 4 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
