/*
 * A379540  Row sums of A376832.
 * https://oeis.org/A379540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - 3*n^3 + 3*n^2 + 3*n - 4)/2 for n>0, a(0)=0
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t sn = (seq_t)n;
    seq_t sn2 = sn * sn;           /* n^2 */
    seq_t sn3 = sn2 * sn;          /* n^3 */
    seq_t sn4 = sn3 * sn;          /* n^4 */
    seq_t term = sn4
              - (seq_t)3 * sn3
              + (seq_t)3 * sn2
              + (seq_t)3 * sn
              - (seq_t)4;
    return term / (seq_t)2;
}
