/*
 * A377165  a(n) = 76 + 275*n.
 * https://oeis.org/A377165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 76 + 275*n
 * Range:     u8 n <= 0, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)76 + (seq_t)275 * (seq_t)n;
}
