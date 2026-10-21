/*
 * A242669  a(n) = n*floor(n/3).
 * https://oeis.org/A242669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*floor(n/3)
 * Range:     u8 n <= 28, u16 n <= 443, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)(n / 3);
}
