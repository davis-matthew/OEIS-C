/*
 * A305074  a(n) = 20*n - 8 (n>=1).
 * https://oeis.org/A305074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*n - 8
 * Range:     u8 n <= 13, u16 n <= 3277, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * 20 - 8;
}
