/*
 * A259395  a(n) = -3*n^2*(n-1)^4*(n+1)*(11*n^3+49*n^2-439*n+171).
 * https://oeis.org/A259395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -3*n^2*(n-1)^4*(n+1)*(11*n^3 + 49*n^2 - 439*n + 171)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 6, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index once to seq_t to avoid intermediate 64‑bit overflow */
    seq_t sn  = (seq_t)n;

    /* compute (n-1) and its fourth power */
    seq_t a   = sn - 1;
    seq_t e   = a * a;
    e         = e * e;          /* e = (n-1)^4 */

    /* polynomial part: 11*n^3 + 49*n^2 - 439*n + 171 */
    seq_t p   = 11 * sn * sn * sn
              + 49 * sn * sn
              - 439 * sn
                + 171;

    /* final product, using the leading factor -3 */
    seq_t result = - ((seq_t)3) * sn * sn * e * (sn + 1) * p;
    return result;
}
