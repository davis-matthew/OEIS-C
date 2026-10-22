/*
 * A386479  a(0) = 0; thereafter a(n) = 2*n^2 - 3*n + 5.
 * https://oeis.org/A386479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 0, otherwise a(n) = 2*n^2 - 3*n + 5
 * Range:     u8 n <= 11, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t nn = (seq_t)n;
    return 2 * nn * nn - 3 * nn + 5;
}
