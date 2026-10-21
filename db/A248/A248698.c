/*
 * A248698  Floor of sums of the non-integer fourth roots of n, as partitioned
 *          by the integer roots: floor(Sum_{j=n^4+1..(n+1)^4-1} j^(1/4)).
 * https://oeis.org/A248698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^4 + 8*n^3 + 8*n^2 + 3*n
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 180, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the sequence type once */
    seq_t nn = (seq_t)n;

    /* Compute successive powers of n */
    seq_t n2 = nn * nn;      /* n^2 */
    seq_t n3 = n2 * nn;      /* n^3 */
    seq_t n4 = n2 * n2;      /* n^4 */

    /* Evaluate the polynomial */
    seq_t result = 4 * n4 + 8 * n3 + 8 * n2 + 3 * nn;
    return result;
}
