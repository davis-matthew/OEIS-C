/*
 * A257645  a(n) = 15*n + 14.
 * https://oeis.org/A257645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*n + 14
 * Range:     u8 n <= 16, u16 n <= 4368, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)15 + (seq_t)14;
}
