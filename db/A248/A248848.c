/*
 * A248848  Norm of coefficients in the expansion of 1/(1 - 3*x - I*x^2),
 *          where I^2=-1.
 * https://oeis.org/A248848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + 2*a(n-2) + 9*a(n-3) - a(n-4) with a(0)=1,
 *            a(1)=9, a(2)=82, a(3)=765
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fast return for the first four terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)82;
    if (n == 3) return (seq_t)765;

    /* Sliding‑window computation using the recurrence */
    seq_t a0 = (seq_t)1,   /* a(n-4)   */
          a1 = (seq_t)9,   /* a(n-3)   */
          a2 = (seq_t)82,  /* a(n-2)   */
          a3 = (seq_t)765; /* a(n-1)   */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = a3 * (seq_t)9   /* 9*a(n-1) */
            + a2 * (seq_t)2   /* 2*a(n-2) */
            + a1 * (seq_t)9   /* 9*a(n-3) */
            - a0;            /* -a(n-4) */

        /* Shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
