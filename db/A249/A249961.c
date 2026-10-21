/*
 * A249961  Number of length 1+5 0..n arrays with no six consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A249961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 3*n^5 + 5*n^4 + 5*n^3 + (3*n^2 - n)/2
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1, but handle 0 gracefully */

    seq_t nn = n;                       /* (seq_t)n */
    seq_t n2 = nn * nn;                /* n^2 */
    seq_t n3 = n2 * nn;                /* n^3 */
    seq_t n4 = n3 * nn;                /* n^4 */
    seq_t n5 = n4 * nn;                /* n^5 */
    seq_t n6 = n5 * nn;                /* n^6 */

    seq_t part6 = n6;
    seq_t part5 = 3 * n5;
    seq_t part4 = 5 * n4;
    seq_t part3 = 5 * n3;
    seq_t part2 = (3 * n2 - nn) / 2;   /* (3*n^2 - n)/2, numerator is even */

    return part6 + part5 + part4 + part3 + part2;
}
