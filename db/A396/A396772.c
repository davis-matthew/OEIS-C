/*
 * A396772  Expansion of x*(-1 + 4*x - 3*x^2 + x^3)/((1 - x)^2 * (1 - x +
 *          x^2)).
 * https://oeis.org/A396772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*floor((n-1)/6) + b[(n-1)%6] (n>0), a(0)=0 where b =
 *            {-1, 1, 4, 6, 6, 5}. This follows from a(n) = n - A057079(n) and
 *            the linear recurrence a(n)=3*a(n-1)-4*a(n-2)+3*a(n-3)-a(n-4)
 * Range:     i8 n <= 128, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    static const seq_t b[6] = { -1, 1, 4, 6, 6, 5 };
    idx_t q = (n - 1) / 6;
    idx_t r = (n - 1) % 6;
    return (seq_t)(q * 6) + b[r];
}
