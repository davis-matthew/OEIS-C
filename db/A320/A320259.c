/*
 * A320259  Terms that are on the y-axis of the square spiral built with 2*k,
 *          2*k+1, 2*k+1 for k >= 0.
 * https://oeis.org/A320259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5), a(0)=0,
 *            a(1)=2, a(2)=5, a(3)=9, a(4)=15
 * Range:     u8 n <= 18, u16 n <= 312, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)15;

    /* Store the last five values: a(n-5), a(n-4), ..., a(n). */
    seq_t a0 = (seq_t)0; /* a(n-5) */
    seq_t a1 = (seq_t)2; /* a(n-4) */
    seq_t a2 = (seq_t)5; /* a(n-3) */
    seq_t a3 = (seq_t)9; /* a(n-2) */
    seq_t a4 = (seq_t)15;/* a(n-1) */
    seq_t cur = a4;      /* placeholder for a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) - a(i-2) + a(i-3) - 2*a(i-4) + a(i-5) */
        cur = 2 * a4 - a3 + a2 - 2 * a1 + a0;

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }

    return cur;
}
