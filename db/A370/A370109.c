/*
 * A370109  a(n) = n^2*(2*n^2-23).
 * https://oeis.org/A370109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^2*(2*n^2-23)
 * Range:     i8 n <= 3, i16 n <= 11, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A370109 for arbitrary integer type seq_t. */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* Convert index to seq_t to avoid truncation */
    seq_t nn2 = nn * nn;           /* n^2  */
    seq_t temp = (seq_t)2 * nn2 - (seq_t)23;  /* 2*n^2 - 23 */
    return nn2 * temp;             /* n^2 * (2*n^2 - 23) */
}
