/*
 * A316386  Binomial transform of [0, 1, 2, -3, -4, 5, 6, -7, -8, ...].
 * https://oeis.org/A316386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-2) + 8*a(n-3) - 4*a(n-4) (n > 3) Initial
 *            values: a(0)=0, a(1)=1, a(2)=4, a(3)=6
 * Range:     i8 n <= 8, i16 n <= 21, i32 n <= 52, i64 n <= 113, i128 n <= 240,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)6;

    /* Keep the last four values only – O(1) space */
    seq_t a0 = (seq_t)0;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)4;   /* a(n-2) */
    seq_t a3 = (seq_t)6;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 4*a(i-1) - 8*a(i-2) + 8*a(i-3) - 4*a(i-4) */
        seq_t next = (seq_t)4 * a3
                   - (seq_t)8 * a2
                   + (seq_t)8 * a1
                   - (seq_t)4 * a0;

        /* Shift values for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
