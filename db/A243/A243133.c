/*
 * A243133  a(n) = 64*n^7 - 112*n^5 + 56*n^3 - 7*n.
 * https://oeis.org/A243133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64*n^7 - 112*n^5 + 56*n^3 - 7*n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 13, u64 n <= 312, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;         /* n^2 */
    seq_t n3 = n2 * nn;         /* n^3 */
    seq_t n4 = n2 * n2;         /* n^4 */
    seq_t n5 = n4 * nn;         /* n^5 */
    seq_t n6 = n4 * n2;         /* n^6 */
    seq_t n7 = n6 * nn;         /* n^7 */
    seq_t result = (64 * n7) - (112 * n5) + (56 * n3) - (7 * nn);
    return result;
}
