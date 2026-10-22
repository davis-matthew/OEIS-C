/*
 * A367015  Number of regions formed after n points have been placed in
 *          general position on each edge of a triangle (as in A365929).
 * https://oeis.org/A367015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^4 - 12*n^3 + 15*n^2 + 4)/4
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 208, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n3  = n2 * nn;
    seq_t n4  = n2 * n2;   /* n^4 */
    seq_t num = 9 * n4 - 12 * n3 + 15 * n2 + 4;
    return num / 4;
}
