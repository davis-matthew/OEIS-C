/*
 * A297662  Number of chordless cycles in the complete tripartite graph
 *          K_{n,n,n}.
 * https://oeis.org/A297662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2*(n-1)^2/4
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }
    seq_t nn = (seq_t)n;
    seq_t nm1 = (seq_t)(n - 1);
    seq_t t = nn * nm1;   /* t = n*(n-1)  */
    t *= t;               /* t = n^2*(n-1)^2  */
    t *= 3;               /* t = 3*n^2*(n-1)^2  */
    t /= 4;               /* t = 3*n^2*(n-1)^2/4  */
    return t;
}
