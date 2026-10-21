/*
 * A242851  a(n) = 64*n^6 - 80*n^4 + 24*n^2 - 1.
 * https://oeis.org/A242851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 64*n^6 - 80*n^4 + 24*n^2 - 1
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 17, i64 n <= 724, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn  = (seq_t)n;
    seq_t t2  = tn * tn;        /* n^2 */
    seq_t t4  = t2 * t2;        /* n^4 */
    seq_t t6  = t4 * t2;        /* n^6 */
    return 64 * t6 - 80 * t4 + 24 * t2 - 1;
}
