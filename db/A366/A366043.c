/*
 * A366043  Number of circular binary sequences of length n with an odd number
 *          of 0's and no consecutive 1's.
 * https://oeis.org/A366043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-3) + a(n-4) with initial values a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=1 (a(1) is the first OEIS term, defined for
 *            n≥1)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the three explicit first terms directly */
    if (n == 0) return (seq_t)0;                 /* unused, safety guard */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)1;

    /* Maintain the four most recent values: a(i-4), a(i-3), a(i-2), a(i-1) */
    seq_t a0 = (seq_t)0;     /* a(i-4) */
    seq_t a1 = (seq_t)1;     /* a(i-3) */
    seq_t a2 = (seq_t)2;     /* a(i-2) */
    seq_t a3 = (seq_t)1;     /* a(i-1) */

    seq_t an;                /* a(i) */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = a(i-2) + 2*a(i-3) + a(i-4) */
        an = a2 + (a1 << 1) + a0;

        /* Shift the window for the next iteration:
           a0 <- a1, a1 <- a2, a2 <- a3, a3 <- an */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return a3;   /* a3 holds a(n) after loop */
}
