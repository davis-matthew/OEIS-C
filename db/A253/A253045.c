/*
 * A253045  a(n) = 8*n^3 - 449*n^2 + 7967*n - 45523.
 * https://oeis.org/A253045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 8*n^3 - 449*n^2 + 7967*n - 45523
 * Range:     i8 none, i16 none, i32 n <= 663, i64 n <= 1000, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute the polynomial term by term; this is exact as long
       as the result fits in seq_t. For larger seq_t types (e.g.
       __int128 or arbitrary-precision) the usual arithmetic rules
       apply. */
    seq_t term = 8 * nn * nn * nn
               - 449 * nn * nn
               + 7967 * nn
               - 45523;
    return term;
}
