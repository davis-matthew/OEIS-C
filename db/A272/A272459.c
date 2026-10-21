/*
 * A272459  The total number of different isosceles trapezoids, excluding
 *          squares, that can be drawn on an n X n square grid where the
 *          corners of each individual trapezoid lie on a lattice point.
 * https://oeis.org/A272459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(-1 - 3*(-1)^n - 12*n + 10*n^2) / 24 (valid for n ≥ 1)
 *            Implemented directly using integer arithmetic
 * Range:     u8 n <= 5, u16 n <= 54, u32 n <= 789, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                   /* a(1) = 0, and a(0) arbitrary */

    seq_t nt = (seq_t)n;             /* cast index to seq_t */
    /* expr = 10*n^2 - 12*n  (always non‑negative for n ≥ 2) */
    seq_t expr = 10 * nt * nt - 12 * nt;

    /* add or subtract 4/2 depending on parity of n */
    if ((n & 1) == 0)                /* n even: subtract 4 */
        expr -= 4;
    else                             /* n odd: add 2 */
        expr += 2;

    /* compute the final result */
    return nt * expr / 24;
}
