/*
 * A272127  a(n) = 1680*n^4 - 168*n^2 + 128*n + 1.
 * https://oeis.org/A272127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1680*n^4 - 168*n^2 + 128*n + 1
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 39, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the proper type first. */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n4 = n2 * n2;          /* n^4 */
    seq_t term1 = 1680 * n4;     /* 1680*n^4 */
    seq_t term2 = 168 * n2;      /* 168*n^2  */
    seq_t term3 = 128 * nn;      /* 128*n    */
    return term1 - term2 + term3 + 1;
}
