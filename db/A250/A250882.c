/*
 * A250882  Number of (5+1) X (n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5328*n^3 + 14468*n^2 + 13238*n + 4096
 * Range:     u8 none, u16 n <= 1, u32 n <= 92, u64 n <= 156, u128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n<1 */
    seq_t nn   = (seq_t)n;
    seq_t nn2  = nn * nn;                 /* n^2  */
    seq_t nn3  = nn2 * nn;                /* n^3  */
    return 5328 * nn3 + 14468 * nn2 + 13238 * nn + 4096;
}
