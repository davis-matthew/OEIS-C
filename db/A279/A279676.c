/*
 * A279676  Coefficients in the expansion of 1/([r] + [2r]x + [3r]x^2 + ...);
 *          [ ] = floor, r = 5/3.
 * https://oeis.org/A279676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1 a(1) = -3 a(2) = 4 a(3) = -3 a(4) = -1 For n ≥ 5 a(n) =
 *            -2·a(n-1) - 2·a(n-2) (derived from the generating function
 *            (1‑x)(1‑x³)/(1+2x+2x²))
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 60, i64 n <= 124,
 *            i128 n <= 252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return -3;
    if (n == 2) return 4;
    if (n == 3) return -3;
    if (n == 4) return -1;

    seq_t a_prevprev = -3;   /* a(3) */
    seq_t a_prev     = -1;   /* a(4) */
    seq_t a_curr = 0;

    for (idx_t i = 5; i <= n; ++i)
    {
        a_curr = (-2)*a_prev + (-2)*a_prevprev;   /* a(i) = -2*a(i-1) - 2*a(i-2) */
        a_prevprev = a_prev;
        a_prev = a_curr;
    }
    return a_prev;
}
