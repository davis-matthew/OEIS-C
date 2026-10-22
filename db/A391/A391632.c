/*
 * A391632  Number of mutual-visibility sets in the n-Andrásfai graph.
 * https://oeis.org/A391632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 * a(1)=4, a(2)=21, a(3)=127, a(4)=749, a(5)=4455,
 * a(n)=10*a(n-1)-29*a(n-2)+32*a(n-3)-8*a(n-4) for n≥6
 */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)21;
    if (n == 3) return (seq_t)127;
    if (n == 4) return (seq_t)749;

    seq_t x1 = (seq_t)4;   /* a(n-4) */
    seq_t x2 = (seq_t)21;  /* a(n-3) */
    seq_t x3 = (seq_t)127; /* a(n-2) */
    seq_t x4 = (seq_t)749; /* a(n-1) */

    /* a(5) has a constant +8 term */
    seq_t a5 = (seq_t)(10 * x4 - 29 * x3 + 32 * x2 - 8 * x1 + 8);
    if (n == 5) return a5;

    /* shift for the homogeneous recurrence */
    x1 = x2;
    x2 = x3;
    x3 = x4;
    x4 = a5;

    seq_t xn = 0;
    for (idx_t i = 6; i <= n; ++i) {
        xn = (seq_t)(10 * x4 - 29 * x3 + 32 * x2 - 8 * x1);
        /* shift for next term */
        x1 = x2;
        x2 = x3;
        x3 = x4;
        x4 = xn;
    }
    return xn;
}
