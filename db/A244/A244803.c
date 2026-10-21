/*
 * A244803  The 360 degree spoke (or ray) of a hexagonal spiral of Ulam.
 * https://oeis.org/A244803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^2 - 25*n + 14
 * Range:     u8 n <= 5, u16 n <= 74, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return (seq_t)12 * tn * tn - (seq_t)25 * tn + (seq_t)14;
}
