/*
 * A263997  Sequence of block lengths in a block spiral of width 1.
 * https://oeis.org/A263997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)/2 for n >= 5; a(1) = 1, a(2) = 2, a(3) = 2, a(4) =
 *            2
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n <= 4)
        return (seq_t)2;
    return (seq_t)((n + 1) / 2);
}
