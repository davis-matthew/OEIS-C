/*
 * A344261  Number of n-step walks from one of the vertices with degree 3 to
 *          itself on the four-vertex diamond graph.
 * https://oeis.org/A344261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - (-1)^n; a(0)=1, a(1)=0
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t a0 = (seq_t)1;   /* a(n-2) */
    seq_t a1 = (seq_t)0;   /* a(n-1) */
    seq_t an, tmp;
    for (idx_t i = 2; i <= n; ++i) {
        tmp = a1 + (seq_t)4 * a0;               /* a(n-1) + 4*a(n-2) */
        if (i & 1)                              /* n odd: add 1 */
            an = tmp + (seq_t)1;
        else                                    /* n even: subtract 1 */
            an = tmp - (seq_t)1;
        a0 = a1;   /* shift for next iteration */
        a1 = an;
    }
    return a1;
}
