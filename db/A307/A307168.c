/*
 * A307168  First class of all proper positive solutions x1(n) = a(n) of the
 *          Pell equation x^2 - 7*y^2 = 9.
 * https://oeis.org/A307168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the Pell equation x^2 - 7*y^2 = 9.
   Proper positive solutions (x_n, y_n) satisfy
        (x_{n+1}, y_{n+1}) = (8*x_n + 21*y_n, 3*x_n + 8*y_n),
   with the initial solution (x_1, y_1) = (11, 4).  
   The sequence a(n) = x_n.  This implementation works for any
   seq_t (8/16/32/64‑bit unsigned or arbitrary‑precision integer)
   and is independent of any external helper functions.  It runs in
   O(n) time and O(1) auxiliary space. */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t x = 11;   /* x_1 */
    seq_t y = 4;    /* y_1 */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t newx = 8 * x + 21 * y;
        seq_t newy = 3 * x + 8 * y;
        x = newx;
        y = newy;
    }
    return x;
}
