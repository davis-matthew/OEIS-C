/*
 * A268973  Number of 3 X n 0..2 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two not
 *          more than once.
 * https://oeis.org/A268973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 29*a(n-2) + 20*a(n-3) - 4*a(n-4) for n>6
 *            initial values: a(1)=27, a(2)=336, a(3)=2016, a(4)=11664,
 *            a(5)=63792, a(6)=339480
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for n = 1..6 */
    static const seq_t base[7] = {0, 27, 336, 2016, 11664, 63792, 339480};

    if (n <= 6)
        return base[n];

    /* sliding window of the last four terms */
    seq_t prev4 = base[3];   /* a(n-4) */
    seq_t prev3 = base[4];   /* a(n-3) */
    seq_t prev2 = base[5];   /* a(n-2) */
    seq_t prev1 = base[6];   /* a(n-1) */
    seq_t term = 0;

    for (idx_t i = 7; i <= n; ++i) {
        seq_t pos = 10 * prev1 + 20 * prev3;         /* 10*a(n-1) + 20*a(n-3) */
        seq_t neg = 29 * prev2 + 4 * prev4;          /* 29*a(n-2) + 4*a(n-4) */
        term = pos - neg;                            /* pos >= neg for all n>6 */

        /* shift the window for next iteration */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = term;
    }
    return term;
}
