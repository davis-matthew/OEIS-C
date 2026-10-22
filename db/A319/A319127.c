/*
 * A319127  Crossing number of the complete bipartite graph K_{6,n}.
 * https://oeis.org/A319127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*floor(n/2)*floor((n-1)/2)
 * Range:     u8 n <= 14, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t m1 = n >> 1;          /* floor(n/2) */
    idx_t m2 = (n - 1) >> 1;    /* floor((n-1)/2) */
    seq_t res = (seq_t)6 * (seq_t)m1 * (seq_t)m2;
    return res;
}
