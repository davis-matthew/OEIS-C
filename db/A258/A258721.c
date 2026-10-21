/*
 * A258721  a(n) = 24*n^2 + 52*n + 29.
 * https://oeis.org/A258721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 24*n^2 + 52*n + 29
 * Range:     u8 n <= 2, u16 n <= 51, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 24 * nn * nn + 52 * nn + 29;
}
