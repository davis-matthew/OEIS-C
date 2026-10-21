/*
 * A269792  a(n) = 5*n^4.
 * https://oeis.org/A269792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^4
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the underlying type once. */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;   /* n^2 */
    seq_t n4 = n2 * n2;   /* n^4 */
    return 5 * n4;        /* 5*n^4 */
}
