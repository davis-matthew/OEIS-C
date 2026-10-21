/*
 * A272870  Real part of (n + i)^4.
 * https://oeis.org/A272870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^4 - 6*n^2 + 1
 * Range:     i8 n <= 3, i16 n <= 13, i32 n <= 215, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t sn2  = sn * sn;      /* n^2 */
    seq_t sn4  = sn2 * sn2;    /* n^4 */
    return sn4 - (seq_t)6 * sn2 + 1;
}
