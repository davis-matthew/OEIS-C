/*
 * A249586  Sum of the first n^3 cubes.
 * https://oeis.org/A249586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n^3 * (n^3 + 1)) / 2)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 45, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type first. */
    seq_t nn = (seq_t)n;

    /* Compute n^3 */
    seq_t n2 = nn * nn;          /* n^2   */
    seq_t n3 = n2 * nn;          /* n^3   */

    /* (n^3) * (n^3 + 1) is even; divide by 2 before squaring */
    seq_t prod = n3 * (n3 + 1);   /* n^3·(n^3+1)  */
    seq_t half = prod / 2;       /* (n^3·(n^3+1))/2 */

    /* Square the result */
    return half * half;
}
