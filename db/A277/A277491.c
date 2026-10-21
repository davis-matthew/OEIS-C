/*
 * A277491  Number of triangles in the standard triangulation of the n-th
 *          approximation of the Koch snowflake fractal.
 * https://oeis.org/A277491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 36*a(n-2), a(0)=1, a(1)=12
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases directly returned. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)12;

    /* Iterative computation of the recurrence. */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)12;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = 13 * prev1 - 36 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
