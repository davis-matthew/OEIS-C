/*
 * A273366  a(n) = 10*n^2 + 10*n + 2.
 * https://oeis.org/A273366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n^2 + 10*n + 2
 * Range:     u8 n <= 4, u16 n <= 80, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t result = tn * tn;          /* n^2   */
    result = result * 10;            /* 10*n^2 */
    seq_t t10 = tn * 10;             /* 10*n  */
    result = result + t10;           /* +10*n */
    result = result + 2;             /* +2    */
    return result;
}
