/*
 * A305069  a(n) = 117*n - 72 (n>=1).
 * https://oeis.org/A305069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 117*n - 72
 * Range:     u8 n <= 2, u16 n <= 560, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */
    return (seq_t)n * 117 - 72;
}
