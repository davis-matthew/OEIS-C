/*
 * A379852  a(n) = floor(8*n^3/27).
 * https://oeis.org/A379852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(8*n^3/27)
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 1285, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t prod = tn * tn;      /* n^2 */
    prod = prod * tn;          /* n^3 */
    prod = prod * 8;           /* 8*n^3 */
    prod = prod / 27;          /* floor(8*n^3/27) */
    return prod;
}
