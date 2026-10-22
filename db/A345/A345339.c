/*
 * A345339  a(n) = 18*n + 20.
 * https://oeis.org/A345339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*n + 20
 * Range:     u8 n <= 13, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n * ((seq_t)18)) + ((seq_t)20);
}
