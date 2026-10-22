/*
 * A391994  Expansion of (x^2*(3*x - 1))/((x - 1)^4*(x + 1)).
 * https://oeis.org/A391994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n^3 - 3*n^2 - n + 3*(n mod 2)) / 6
 * Range:     i8 n <= 6, i16 n <= 33, i32 n <= 1496, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;
    seq_t n3   = n2 * nn;
    seq_t mod2 = (seq_t)(n & 1U);
    seq_t num  = n3 - 3 * n2 - nn + 3 * mod2;
    return num / 6;
}
