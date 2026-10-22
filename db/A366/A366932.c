/*
 * A366932  Total number of edges formed after n points have been placed in
 *          general position on each edge of a triangle (as in A365929).
 * https://oeis.org/A366932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(3*n^4/2 - 2*n^3 + 3*n^2/2 + n + 1)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 175, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer form: a(n) = (9*n^4 - 12*n^3 + 9*n^2 + 6*n + 6) / 2 */

seq_t a(idx_t n)
{
    /* convert idx_t to seq_t as required */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n3 = n2 * sn;          /* n^3 */
    seq_t n4 = n3 * sn;          /* n^4 */

    /* numerator: 9*n^4 - 12*n^3 + 9*n^2 + 6*n + 6  */
    seq_t num = (seq_t)9 * n4
              - (seq_t)12 * n3
              + (seq_t)9 * n2
              + (seq_t)6 * sn
              + (seq_t)6;

    /* division by 2 is exact for integer n */
    return num / (seq_t)2;
}
