/*
 * A255877  a(n) = (2n-2)^3 + (2n-2) - 1.
 * https://oeis.org/A255877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n-2)^3 + (2*n-2) - 1
 * Range:     i8 n <= 3, i16 n <= 16, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t t   = nseq * 2 - 2;   /* 2*n - 2   */
    seq_t t2  = t * t;          /* (2*n - 2)^2 */
    seq_t t3  = t2 * t;         /* (2*n - 2)^3 */
    return t3 + t - 1;          /* result */
}
