/*
 * A295622  Number of nonequivalent dissections of an n-gon into 3 polygons by
 *          nonintersecting diagonals rooted at a cell up to rotation.
 * https://oeis.org/A295622
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-4)*(-5 + (-1)^n - 4*n + 2*n^2) / 8 for n>4
 * Range:     u8 n <= 7, u16 n <= 34, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 5) return (seq_t)0;                 /* value below offset is irrelevant */

    seq_t sn = (seq_t)n;                         /* convert index to seq_t */

    /* expr = 2*n^2 - 4*n - 5 + (-1)^n */
    seq_t expr = (seq_t)2 * sn * sn;             /* 2*n^2   */
    expr -= (seq_t)4 * sn;                      /* -4*n    */
    expr -= (seq_t)5;                           /* -5      */

    /* add (-1)^n : +1 if n even, -1 if n odd */
    if ((n & 1) == 0)
        expr += (seq_t)1;                       /* (+1)    */
    else
        expr -= (seq_t)1;                       /* (-1)    */

    seq_t mult = (sn - (seq_t)4) * expr;        /* (n-4)*expr */
    return mult / (seq_t)8;                     /* divide by 8 */
}
