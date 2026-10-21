/*
 * A263231  a(n) = n*(25*n - 39)/2.
 * https://oeis.org/A263231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(25*n - 39)/2
 * Range:     i8 n <= 3, i16 n <= 36, i32 n <= 9493, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert the index once, then do all arithmetic in seq_t */
    seq_t nn = (seq_t)n;
    seq_t res = nn * ((nn * 25) - 39);
    res /= 2;
    return res;
}
