/*
 * A242436  a(n) = n^5 - 2n.
 * https://oeis.org/A242436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^5 - 2n
 * Range:     i8 n <= 2, i16 n <= 8, i32 n <= 73, i64 n <= 6208,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;        /* n */
    seq_t n2 = nn * nn;         /* n^2 */
    seq_t n3 = n2 * nn;         /* n^3 */
    seq_t n4 = n3 * nn;         /* n^4 */
    seq_t n5 = n4 * nn;         /* n^5 */
    seq_t two_n = (seq_t)2 * nn;/* 2n */
    return n5 - two_n;          /* n^5 - 2n */
}
