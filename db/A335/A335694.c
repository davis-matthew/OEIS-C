/*
 * A335694  a(n) = 3*binomial(n,4) - 6*binomial(n,3) + 4*binomial(n,2) - 2.
 * https://oeis.org/A335694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n^4 - 14*n^3 + 51*n^2 - 38*n - 16) / 8
 * Range:     i8 n <= 5, i16 n <= 17, i32 n <= 257, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;          /* cast index to seq_t */
    seq_t n2  = n_t * n_t;          /* n^2 */
    seq_t n3  = n2 * n_t;           /* n^3 */
    seq_t n4  = n3 * n_t;           /* n^4 */
    seq_t num = n4 - 14 * n3 + 51 * n2 - 38 * n_t - 16;
    return num / 8;
}
