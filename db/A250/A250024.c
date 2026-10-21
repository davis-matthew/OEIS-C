/*
 * A250024  a(n) = 40*n - 21.
 * https://oeis.org/A250024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40*n - 21
 * Range:     u8 n <= 6, u16 n <= 1638, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)40 - (seq_t)21;
}
