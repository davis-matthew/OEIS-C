/*
 * A298267  a(n) is the maximum number of heptiamonds in a hexagon of order n.
 * https://oeis.org/A298267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((6*(n + 1)^2)/7)
 * Range:     u8 n <= 16, u16 n <= 275, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)6 * (n + 1) * (n + 1)) / 7;
}
