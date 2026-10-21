/*
 * A292290  Number of vertices of type A at level n of the hyperbolic Pascal
 *          pyramid.
 * https://oeis.org/A292290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3) (n >= 4) Initial values:
 *            a(0)=0, a(1)=0, a(2)=3, a(3)=6
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* handle first four terms directly */
    if (n == 0 || n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)6;

    /* iterate using the recurrence */
    seq_t a1 = (seq_t)0;      /* a(n-3)  */
    seq_t a2 = (seq_t)3;      /* a(n-2)  */
    seq_t a3 = (seq_t)6;      /* a(n-1)  */
    seq_t an = a3;            /* placeholder for a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        an = 4 * a3 - 4 * a2 + a1;   /* a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3) */
        a1 = a2;  /* shift for next iteration */
        a2 = a3;
        a3 = an;
    }
    return a3;
}
