/*
 * A253513  The characteristic function of the multiples of eight.
 * https://oeis.org/A253513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/8) - floor((n-1)/8)
 * Range:     u8 n <= 9024, u16 n <= 9024, u32 n <= 9024, u64 n <= 9024,
 *            u128 n <= 9024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)((n & 7ull) == 0));
}
