/*
 * A269053  Number of 2 X n 0..2 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-3) - 4*a(n-4); a(1)=9, a(2)=42,
 *            a(3)=102, a(4)=360
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset starts at 1; undefined for n < 1 */
    /* Base values for n = 1,2,3,4 */
    const seq_t base1 = (seq_t)9;
    const seq_t base2 = (seq_t)42;
    const seq_t base3 = (seq_t)102;
    const seq_t base4 = (seq_t)360;

    if (n == 1) return base1;
    if (n == 2) return base2;
    if (n == 3) return base3;
    if (n == 4) return base4;

    /* Iteratively compute values using the recurrence */
    seq_t d1 = base1;   /* a(n-4) */
    seq_t d2 = base2;   /* a(n-3) */
    seq_t d3 = base3;   /* a(n-2) */
    seq_t d4 = base4;   /* a(n-1) */
    seq_t an = 0;

    for (idx_t k = 5; k <= n; ++k) {
        an = 4 * d4 - 8 * d2 - 4 * d1;
        /* shift the window of last four values */
        d1 = d2;
        d2 = d3;
        d3 = d4;
        d4 = an;
    }
    return an;
}
