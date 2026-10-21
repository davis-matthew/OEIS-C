/*
 * A247365  Central terms of triangles A102472 and A102473.
 * https://oeis.org/A247365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = y(n,n), where for a fixed n y(0,n) = 0, y(1,n) = 1
 *            y(m+2,n) = (m + n) * y(m+1,n) + y(m,n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n <= 1)
        return 1;

    seq_t y_pp = 0;  /* y(0,n) */
    seq_t y_p  = 1;  /* y(1,n) */

    for (idx_t m = 0; m + 2 <= n; ++m)
    {
        seq_t mn = (seq_t)m + (seq_t)n;
        seq_t y_next = mn * y_p + y_pp;
        y_pp = y_p;
        y_p  = y_next;
    }

    return y_p;    /* y(n,n) */
}
