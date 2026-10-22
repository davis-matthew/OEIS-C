/*
 * A395260  Number of order-convex subsets of the rectangular grid poset [2] X
 *          [n] with componentwise partial order.
 * https://oeis.org/A395260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 4*n^3 + 17*n^2 + 14*n + 12)/12
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 474, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;                      /* n^2 */
    seq_t n3  = n2 * nn;                      /* n^3 */
    seq_t n4  = n3 * nn;                      /* n^4 */
    seq_t sum = n4
              + (seq_t)4 * n3
              + (seq_t)17 * n2
              + (seq_t)14 * nn
              + (seq_t)12;
    return sum / (seq_t)12;
}
