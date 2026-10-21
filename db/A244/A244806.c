/*
 * A244806  The 180-degree spoke (or ray) of a hexagonal spiral of Ulam.
 * https://oeis.org/A244806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^2 - 19*n + 8
 * Range:     u8 n <= 5, u16 n <= 74, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t result = 12 * nn * nn - 19 * nn + 8;
    return result;
}
