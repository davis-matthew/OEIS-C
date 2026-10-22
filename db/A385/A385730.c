/*
 * A385730  The number of lattice points in the 2D plane contained between the
 *          curve y=x^2/4 and the line y=n^2, inclusive, where n is a positive
 *          integer.
 * https://oeis.org/A385730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/3)*(8*n^3 + 7*n + 3)
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 904, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the result type first. */
    seq_t nn = (seq_t)n;
    /* Compute n^2 and n^3 without intermediate big integers. */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    /* Evaluate the numerator 8*n^3 + 7*n + 3. */
    seq_t num = (seq_t)8 * n3 + (seq_t)7 * nn + (seq_t)3;
    /* Integer division by 3 gives the desired term. */
    return num / (seq_t)3;
}
