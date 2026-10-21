/*
 * A242145  Number of length 1+5 0..n arrays with no consecutive six elements
 *          summing to more than 3*n.
 * https://oeis.org/A242145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (20*n^6 + 131*n^5 + 355*n^4 + 515*n^3 + 425*n^2 + 194*n +
 *            40)/40
 * Range:     u8 none, u16 n <= 2, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute powers of n */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */
    seq_t n5 = n4 * nn;          /* n^5 */
    seq_t n6 = n5 * nn;          /* n^6 */

    /* Compute the numerator (all terms are integers) */
    seq_t numer = 20 * n6
                + 131 * n5
                + 355 * n4
                + 515 * n3
                + 425 * n2
                + 194 * nn
                + 40;

    /* Divide by 40 to obtain the term a(n) */
    return numer / 40;
}
