/*
 * A386486  a(0) = 1; thereafter a(n) = 4*n^2 - 3*n + 2.
 * https://oeis.org/A386486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 4*n^2 - 3*n + 2 for n > 0
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    return nn * (((seq_t)4 * nn) - (seq_t)3) + (seq_t)2;
}
