/*
 * A266958  Numbers m such that 9*m+13 is a square.
 * https://oeis.org/A266958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (18*(n-1)*n + (2*n-1)*(-1)^n - 7)/8 (n ≥ 1)
 * Range:     i8 n <= 3, i16 n <= 43, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t first to avoid overflows in idx_t arithmetic */
    seq_t nt = (seq_t)n;

    /* term1 = 18*(n-1)*n */
    seq_t term1 = (seq_t)18 * (nt - 1) * nt;

    /* term2 = (2*n-1)*(-1)^n */
    seq_t power = (n & 1) ? (seq_t)-1 : (seq_t)1;  /* (-1)^n */
    seq_t term2 = (seq_t)(2) * nt - 1;
    term2 *= power;

    /* numerator = term1 + term2 - 7 */
    seq_t numerator = term1 + term2 - 7;

    /* divide by 8 */
    return numerator / 8;
}
