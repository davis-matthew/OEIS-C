/*
 * A337780  a(n)/A163403(n) = the total length of the lines drawn at
 *          generation n for Conant's dissection of a square with size 1.
 * https://oeis.org/A337780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, changed
 * Formula:   a(1)=1, a(2)=1, a(3)=3, a(4)=5, a(5)=9, a(6)=17, a(7)=37 and for
 *            n≥8 a(n) = a(n-1) + 2*a(n-2) – 2*a(n-3) + 4*a(n-4) – 4*a(n-6) +
 *            4*a(n-7)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0; /* undefined for n<1 */

    /* initial seven values */
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)1;
    seq_t a3 = (seq_t)3;
    seq_t a4 = (seq_t)5;
    seq_t a5 = (seq_t)9;
    seq_t a6 = (seq_t)17;
    seq_t a7 = (seq_t)37;

    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;
    if (n == 5) return a5;
    if (n == 6) return a6;
    if (n == 7) return a7;

    /* sliding window of the last seven terms:
       indices: 0→a_{i-7}, 1→a_{i-6}, 2→a_{i-5},
                3→a_{i-4}, 4→a_{i-3}, 5→a_{i-2}, 6→a_{i-1}
       current = a_i
    */
    seq_t prev[7] = {a1, a2, a3, a4, a5, a6, a7};
    seq_t current;

    for (idx_t i = 8; i <= n; ++i)
    {
        current = prev[6]                         /* a(i-1) */
                + (seq_t)2 * prev[5]              /* +2*a(i-2) */
                - (seq_t)2 * prev[4]              /* -2*a(i-3) */
                + (seq_t)4 * prev[3]              /* +4*a(i-4) */
                - (seq_t)4 * prev[1]              /* -4*a(i-6) */
                + (seq_t)4 * prev[0];             /* +4*a(i-7) */

        /* shift window for next iteration */
        for (int k = 0; k < 6; ++k)
            prev[k] = prev[k + 1];
        prev[6] = current;
    }

    return current;
}
