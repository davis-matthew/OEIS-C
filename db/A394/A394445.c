/*
 * A394445  a(n) is the minimum k such that every 2-coloring of {1, ..., k}
 *          contains a monochromatic solution to x + y = n*z with x, y, z
 *          pairwise distinct.
 * https://oeis.org/A394445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9, 9, 15, 20, 25, 31, 49 for n = 1 … 7 a(n) = n*(n+3)/2
 *            for n ≥ 8 and n odd a(n) = (n²+2n+2)/2 for n ≥ 8 and n even
 * Range:     u8 n <= 21, u16 n <= 360, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first seven terms explicitly */
    if (n <= 7)
    {
        /* constants of type seq_t */
        static const seq_t small[] = {
            (seq_t)9,  /* n = 1 */
            (seq_t)9,  /* n = 2 */
            (seq_t)15, /* n = 3 */
            (seq_t)20, /* n = 4 */
            (seq_t)25, /* n = 5 */
            (seq_t)31, /* n = 6 */
            (seq_t)49  /* n = 7 */
        };
        return small[n - 1];
    }

    /* for n ≥ 8 use the closed formulas */
    seq_t nn = (seq_t)n;
    if ((nn & (seq_t)1) != 0)   /* odd n */
    {
        return nn * (nn + (seq_t)3) / (seq_t)2;
    }
    else                         /* even n */
    {
        return (nn * nn + (seq_t)2 * nn + (seq_t)2) / (seq_t)2;
    }
}
