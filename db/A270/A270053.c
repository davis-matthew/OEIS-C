/*
 * A270053  Number of nX3 0..4 arrays with some element plus some horizontally
 *          or vertically adjacent neighbor totalling four exactly once.
 * https://oeis.org/A270053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(n) = 90*a(n-1) - 2257*a(n-2) + 10440*a(n-3) -
 *            13456*a(n-4) for n > 5
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1) = 40, a(2) = 5400, a(3) = 385200, a(4) = 22911300, a(5) = 1248460920 */

seq_t a(idx_t n)
{
    static const seq_t init[5] = {
        (seq_t)40,
        (seq_t)5400,
        (seq_t)385200,
        (seq_t)22911300,
        (seq_t)1248460920
    };

    if (n == 0)
        return (seq_t)0;   /* undefined, but return 0 */

    if (n <= 5)
        return init[n - 1];

    /* a1 = a(n-4), a2 = a(n-3), a3 = a(n-2), a4 = a(n-1) for the current n */
    seq_t a1 = init[1];  /* a(2) */
    seq_t a2 = init[2];  /* a(3) */
    seq_t a3 = init[3];  /* a(4) */
    seq_t a4 = init[4];  /* a(5) */

    seq_t an = 0;
    for (idx_t k = 6; k <= n; ++k) {
        an = (seq_t)90 * a4
            - (seq_t)2257 * a3
            + (seq_t)10440 * a2
            - (seq_t)13456 * a1;

        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
