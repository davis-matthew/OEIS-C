/*
 * A364352  a(n) is the number of regions into which the plane is divided by n
 *          lines parallel to each edge of an equilateral triangle with side n
 *          such that the lines extend the parallel edge and divide the other
 *          edges into unit segments.
 * https://oeis.org/A364352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5*n + 3)/2 + 3
 * Range:     u8 n <= 9, u16 n <= 161, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = (nn * (5 * nn + 3)) / 2 + 3;
    return val;
}
