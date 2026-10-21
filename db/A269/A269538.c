/*
 * A269538  Number of length-4 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than one.
 * https://oeis.org/A269538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 4*n^3 + 3*n^2 + 2*n
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t n2 = m * m;     /* n^2 */
    seq_t n3 = n2 * m;    /* n^3 */
    seq_t n4 = n2 * n2;   /* n^4 */
    return n4 + 4 * n3 + 3 * n2 + 2 * m;
}
