/*
 * A295853  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = -2,
 *          a(1) = -1, a(2) = 2, a(3) = 1.
 * https://oeis.org/A295853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4), * a(0) = -2,
 *            a(1) = -1, a(2) = 2, a(3) = 1
 * Range:     i8 n <= 7, i16 n <= 18, i32 n <= 41, i64 n <= 87, i128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 0) return (seq_t)-2;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)1;

    /* Current values: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = (seq_t)-2;   /* a(n-4) */
    seq_t a1 = (seq_t)-1;   /* a(n-3) */
    seq_t a2 = (seq_t)2;    /* a(n-2) */
    seq_t a3 = (seq_t)1;    /* a(n-1) */

    seq_t next;
    for (idx_t i = 4; i <= n; ++i) {
        /* a(n) = a(n-1)+3*a(n-2)-2*a(n-3)-2*a(n-4) */
        next = a3 + (seq_t)3 * a2 - (seq_t)2 * a1 - (seq_t)2 * a0;

        /* Shift the window forward for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
