/*
 * A270145  Number of n X 2 0..7 arrays with some element plus some
 *          horizontally, diagonally, antidiagonally or vertically adjacent
 *          neighbor totalling seven exactly once.
 * https://oeis.org/A270145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 66*a(n-1) - 1021*a(n-2) - 2244*a(n-3) - 1156*a(n-4) for
 *            n>5 with a(1)=8, a(2)=1440, a(3)=85056, a(4)=4126368,
 *            a(5)=182257536
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0) return 0;               /* not defined for n<1, but harmless */
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)1440;
    if (n == 3) return (seq_t)85056;
    if (n == 4) return (seq_t)4126368;
    if (n == 5) return (seq_t)182257536;

    /* iterate using the recurrence */
    seq_t a1 = (seq_t)8;                 /* a(n-5) */
    seq_t a2 = (seq_t)1440;              /* a(n-4) */
    seq_t a3 = (seq_t)85056;             /* a(n-3) */
    seq_t a4 = (seq_t)4126368;           /* a(n-2) */
    seq_t a5 = (seq_t)182257536;         /* a(n-1) */

    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t an =
            (seq_t)66 * a5
            - ((seq_t)1021 * a4
                + (seq_t)2244 * a3
                + (seq_t)1156 * a2);
        /* shift values for next iteration */
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }

    return a5;
}
