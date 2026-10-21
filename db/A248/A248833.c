/*
 * A248833  The curvature of touching circles inscribed in a special way in
 *          the larger segment of circle of radius 1/6 divided by a chord of
 *          length sqrt(8/75).
 * https://oeis.org/A248833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 9*a(n-2) + a(n-3); with a(0)=10, a(1)=25,
 *            a(2)=160
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values */
    seq_t a0 = (seq_t)10;   /* a(n-3) */
    seq_t a1 = (seq_t)25;   /* a(n-2) */
    seq_t a2 = (seq_t)160;  /* a(n-1) */

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;

    /* iterate the recurrence up to n */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = 9 * a2 - 9 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
