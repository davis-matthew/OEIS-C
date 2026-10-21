/*
 * A269609  Number of length-6 0..n arrays with no repeated value differing
 *          from the previous repeated value by one or less.
 * https://oeis.org/A269609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 6*n^5 + 11*n^4 - 8*n^3 + n^2 + 3*n - 2
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* n as seq_t */
    seq_t n2 = nn * nn;                 /* n^2 */
    seq_t n3 = n2 * nn;                 /* n^3 */
    seq_t n4 = n3 * nn;                 /* n^4 */
    seq_t n5 = n4 * nn;                 /* n^5 */
    seq_t n6 = n5 * nn;                 /* n^6 */

    seq_t result = n6;
    result += 6 * n5;                   /* + 6*n^5 */
    result += 11 * n4;                  /* + 11*n^4 */
    result -= 8 * n3;                   /* - 8*n^3 */
    result += n2;                       /* + n^2 */
    result += 3 * nn;                   /* + 3*n */
    result -= 2;                        /* - 2 */

    return result;
}
