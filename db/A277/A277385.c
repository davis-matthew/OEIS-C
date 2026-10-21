/*
 * A277385  Records in A277384.
 * https://oeis.org/A277385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 + 12*n - 7 (a(1)=15)
 * Range:     u8 n <= 6, u16 n <= 126, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset, safe guard */
    if (n == 1) return 15;          /* first term */
    seq_t nn = (seq_t)n;
    return (seq_t)4 * nn * nn + (seq_t)12 * nn - (seq_t)7;
}
