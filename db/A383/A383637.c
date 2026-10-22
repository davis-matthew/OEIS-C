/*
 * A383637  Expansion of 1/((1-x) * (1+3*x) * (1-5*x)).
 * https://oeis.org/A383637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 13*a(n-2) - 15*a(n-3), a(0)=1, a(1)=3, a(2)=22
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)22;

    /* Variables to hold the last three computed values. */
    seq_t a0 = (seq_t)1;   /* a(n-3)  */
    seq_t a1 = (seq_t)3;   /* a(n-2)  */
    seq_t a2 = (seq_t)22;  /* a(n-1)  */

    /* Iterate up to the requested index. */
    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 3*a(i-1) + 13*a(i-2) - 15*a(i-3)  */
        seq_t next = (a2 * 3) + (a1 * 13) - (a0 * 15);

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
