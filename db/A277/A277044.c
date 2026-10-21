/*
 * A277044  Number of 2 X 2 matrices with entries in {0,1,...,n} and even
 *          determinant with no entry repeated.
 * https://oeis.org/A277044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^4 - 8*n^3 + 4*n^2 - 16*n)/8 for even n a(n) = (5*n^4
 *            - 12*n^3 + 2*n^2 + 12*n - 7)/8 for odd n
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 195, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */
    seq_t val;

    if ((n & 1) == 0) {          /* even n */
        val = ((seq_t)5 * n4
             - (seq_t)8 * n3
             + (seq_t)4 * n2
             - (seq_t)16 * nn);
    } else {                     /* odd n */
        val = ((seq_t)5 * n4
             - (seq_t)12 * n3
             + (seq_t)2 * n2
             + (seq_t)12 * nn
             - (seq_t)7);
    }

    return val / (seq_t)8;
}
