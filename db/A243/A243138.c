/*
 * A243138  a(n) = n^2 + 15*n + 13.
 * https://oeis.org/A243138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 15*n + 13
 * Range:     u8 n <= 9, u16 n <= 248, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return x * x + (seq_t)15 * x + (seq_t)13;
}
