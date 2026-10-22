/*
 * A322039  Expansion of (1 + x)^2 / ((1 - x)^2*(1 + 2*x)^2).
 * https://oeis.org/A322039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + 3*a(n-2) + 4*a(n-3) - 4*a(n-4) (n > 3)
 *            Initial values: a(0) = 1, a(1) = 0, a(2) = 4, a(3) = -4
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 29, i64 n <= 60, i128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (n == 2) return 4;
    if (n == 3) return -4;

    seq_t a0 = 1, a1 = 0, a2 = 4, a3 = -4;
    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t an = (-2) * a3 + 3 * a2 + 4 * a1 - 4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
