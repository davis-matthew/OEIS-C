/*
 * A272130  a(n) = 16*n^3 + 10*n^2 + 4*n + 1.
 * https://oeis.org/A272130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^3 + 10*n^2 + 4*n + 1
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 644, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;          /* cast n to seq_t */
    seq_t n_sq = nn * nn;  /* n^2 */
    /* compute a(n) = 16*n^3 + 10*n^2 + 4*n + 1 */
    return 16 * nn * n_sq + 10 * n_sq + 4 * nn + 1;
}
