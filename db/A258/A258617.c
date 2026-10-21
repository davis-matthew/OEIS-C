/*
 * A258617  a(n) = (4*n+8)*n^2.
 * https://oeis.org/A258617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+8)*n^2
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t n_sq = nn * nn;         /* n^2 */
    seq_t factor = 4 * nn + 8;    /* 4*n + 8 */
    return n_sq * factor;         /* (4*n+8)*n^2 */
}
