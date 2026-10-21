/*
 * A269410  Number of length-4 0..n arrays with no repeated value greater than
 *          or equal to the previous repeated value.
 * https://oeis.org/A269410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 4*n^3 + (7/2)*n^2 + (1/2)*n (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* behaviour for n < 1 does not matter */

    seq_t nn   = (seq_t)n;        /* n */
    seq_t n2   = nn * nn;         /* n^2 */
    seq_t n3   = n2 * nn;         /* n^3 */
    seq_t n4   = n3 * nn;         /* n^4 */

    /* compute 2*n^4 + 8*n^3 + 7*n^2 + n, then divide by 2 */
    seq_t numerator = (seq_t)2 * n4 + (seq_t)8 * n3 + (seq_t)7 * n2 + nn;

    return numerator / (seq_t)2;
}
