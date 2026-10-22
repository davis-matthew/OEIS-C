/*
 * A316671  Squares visited by moving diagonally one square on a diagonally
 *          numbered board and moving to the lowest available unvisited square
 *          at each step.
 * https://oeis.org/A316671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + n + 4)/2 for n even * a(n) = (n^2 - n + 2)/2 for n
 *            odd
 * Range:     u8 n <= 15, u16 n <= 360, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t before arithmetic to avoid overflow in intermediate idx_t operations */
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;          /* n^2 */
    if ((n & 1) == 0)            /* even */
        return (sq + sn + (seq_t)4) / 2;
    else                         /* odd */
        return (sq - sn + (seq_t)2) / 2;
}
