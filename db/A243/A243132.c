/*
 * A243132  a(n) = 32*n^6 - 48*n^4 + 18*n^2 - 1.
 * https://oeis.org/A243132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 32*n^6 - 48*n^4 + 18*n^2 - 1
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 20, i64 n <= 812, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn  = (seq_t)n;
    seq_t n2  = tn * tn;          /* n^2 */
    seq_t n4  = n2 * n2;          /* n^4 */
    seq_t n6  = n4 * n2;          /* n^6 */
    return (seq_t)32 * n6 - (seq_t)48 * n4 + (seq_t)18 * n2 - (seq_t)1;
}
