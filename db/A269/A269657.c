/*
 * A269657  Number of length-4 0..n arrays with no adjacent pair x,x+1
 *          repeated.
 * https://oeis.org/A269657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^4 - n = n^4 + 4*n^3 + 6*n^2 + 3*n + 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t m = n_seq + 1;
    seq_t res = m * m;   /* (n+1)^2 */
    res = res * res;     /* (n+1)^4 */
    res -= n_seq;        /* subtract n */
    return res;
}
