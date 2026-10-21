/*
 * A262402  a(n) = number of triangles that can be formed from the points of a
 *          3 X n grid.
 * https://oeis.org/A262402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(8*n-7)/2 (integer division)
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 870, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2    = n_seq * n_seq;          /* n^2          */
    seq_t tmp   = (seq_t)8 * n_seq - (seq_t)7;  /* 8*n - 7  */
    seq_t prod  = n2 * tmp;               /* n^2*(8*n-7)   */
    return prod / 2;                      /* integer division */
}
