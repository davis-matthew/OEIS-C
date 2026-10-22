/*
 * A398559  a(n) = n^4 - 99*n^3 + 3441*n^2 - 49259*n + 247889, for n >= 0.
 * https://oeis.org/A398559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^4 - 99*n^3 + 3441*n^2 - 49259*n + 247889
 * Range:     i8 none, i16 none, i32 n <= 240, i64 n <= 10000, i128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;       /* promote index to the sequence type */
    seq_t res = 1;             /* start with leading coefficient of n^4 */

    /* Horner’s rule: ((((1)*n + (-99))*n + 3441)*n + (-49259))*n + 247889 */
    res = res * ns + (seq_t)(-99);
    res = res * ns + (seq_t)3441;
    res = res * ns + (seq_t)(-49259);
    res = res * ns + (seq_t)247889;

    return res;
}
