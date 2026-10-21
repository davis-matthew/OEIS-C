/*
 * A247854  The 9th Hermite Polynomial evaluated at n: H_9(n) = 512*n^9 -
 *          9216*n^7 + 48384*n^5 - 80640*n^3 + 30240*n.
 * https://oeis.org/A247854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 512*n^9 - 9216*n^7 + 48384*n^5 - 80640*n^3 + 30240*n
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 5, i64 n <= 64, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t x5 = x3 * x2;
    seq_t x7 = x5 * x2;
    seq_t x9 = x7 * x2;
    seq_t result = (seq_t)512 * x9 - (seq_t)9216 * x7 + (seq_t)48384 * x5
                 - (seq_t)80640 * x3 + (seq_t)30240 * x;
    return result;
}
