/*
 * A389608  a(n) = (7*n^2 - 5*n + 2)/2.
 * https://oeis.org/A389608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n^2 - 5*n + 2)/2
 * Range:     u8 n <= 8, u16 n <= 137, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return ((7 * nn * nn) - (5 * nn) + 2) / 2;
}
