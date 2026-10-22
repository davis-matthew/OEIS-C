/*
 * A365606  Number of degree 2 vertices in the n-Sierpinski carpet graph.
 * https://oeis.org/A365606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 35*a(n-2) + 24*a(n-3) Initial values: a(1)=8,
 *            a(2)=20, a(3)=84 This recurrence is equivalent to the closed
 *            form a(n) = (3*8^n + 32*3^n + 120)/30
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)84;

    /* a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) for the loop */
    seq_t a1 = (seq_t)8;   /* a(1) */
    seq_t a2 = (seq_t)20;  /* a(2) */
    seq_t a3 = (seq_t)84;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) = 12*a3 - 35*a2 + 24*a1 */
        seq_t next = (seq_t)(12) * a3 - (seq_t)(35) * a2 + (seq_t)(24) * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
