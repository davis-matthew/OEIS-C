/*
 * A385106  a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4) with a(1) = 1, a(2) =
 *          2, a(3) = 4, and a(4) = 7.
 * https://oeis.org/A385106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4), with a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=7
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* value below the offset is irrelevant */

    if (n == 1)
        return 1;
    if (n == 2)
        return 2;
    if (n == 3)
        return 4;
    if (n == 4)
        return 7;

    /* Keep the last four values in a circular buffer */
    seq_t buf[4];
    buf[0] = 1;   /* a(1) */
    buf[1] = 2;   /* a(2) */
    buf[2] = 4;   /* a(3) */
    buf[3] = 7;   /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        idx_t i1 = (i - 2) & 3;   /* a(i-1) */
        idx_t i2 = (i - 3) & 3;   /* a(i-2) */
        idx_t i3 = (i - 4) & 3;   /* a(i-3) */
        idx_t i4 = (i - 5) & 3;   /* a(i-4) */

        seq_t val = 3 * buf[i1] - 3 * buf[i2] + buf[i3] + buf[i4];
        buf[(i - 1) & 3] = val;   /* store a(i) */
    }

    return buf[(n - 1) & 3];
}
