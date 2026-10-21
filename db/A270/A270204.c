/*
 * A270204  a(n) = n^12 - n^10 + n^8 - n^6 + n^4 - n^2 + 1.
 * https://oeis.org/A270204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^12 - n^10 + n^8 - n^6 + n^4 - n^2 + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 40, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;        /* n^2   */
    seq_t n4 = n2 * n2;        /* n^4   */
    seq_t n6 = n4 * n2;        /* n^6   */
    seq_t n8 = n6 * n2;        /* n^8   */
    seq_t n10 = n8 * n2;       /* n^10  */
    seq_t n12 = n10 * n2;      /* n^12  */
    return n12 - n10 + n8 - n6 + n4 - n2 + (seq_t)1;
}
