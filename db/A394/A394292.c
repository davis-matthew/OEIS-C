/*
 * A394292  Number of induced cubes in the n X n giraffe graph.
 * https://oeis.org/A394292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*(29 - 10*n + n^2) for n >= 10. For 1 <= n <= 9 the
 *            values are obtained from a small lookup array
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined offset, return 0 */
        return (seq_t)0;

    if (n <= 9) {
        static const seq_t tbl[9] = {
            1,   /* a(1) */
            4,   /* a(2) */
            9,   /* a(3) */
            16,  /* a(4) */
            41,  /* a(5) */
            80,  /* a(6) */
            137, /* a(7) */
            212, /* a(8) */
            319  /* a(9) */
        };
        return tbl[n - 1];
    }

    seq_t nn = (seq_t)n;
    seq_t temp = nn * nn;          /* n^2 */
    temp -= (seq_t)10 * nn;        /* -10*n */
    temp += (seq_t)29;             /* +29 */
    return (seq_t)16 * temp;       /* multiply by 16 */
}
