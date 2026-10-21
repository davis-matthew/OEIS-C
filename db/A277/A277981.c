/*
 * A277981  a(n) = 4*n^2 + 18*n - 20.
 * https://oeis.org/A277981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*n^2 + 18*n - 20
 * Range:     i8 n <= 4, i16 n <= 88, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type before any arithmetic */
    seq_t sn = (seq_t)n;
    return 4 * sn * sn + 18 * sn - 20;
}
