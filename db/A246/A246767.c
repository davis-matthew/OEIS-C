/*
 * A246767  a(n) = n^4 - 2n.
 * https://oeis.org/A246767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n-1)^4 - 2*(n-1) (with offset 1)
 * Range:     i8 n <= 4, i16 n <= 14, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n - 1;   /* m = n-1 */
    seq_t m2 = m * m;         /* m^2 */
    seq_t m4 = m2 * m2;       /* m^4 */
    return m4 - 2 * m;        /* m^4 - 2m = (n-1)^4 - 2(n-1) */
}
