/*
 * A321195  Minimum number of monochromatic Schur triples over all 2-colorings
 *          of [n].
 * https://oeis.org/A321195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n^2 - 4*n + 6) / 11)
 * Range:     u8 n <= 17, u16 n <= 282, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = nn * nn - (seq_t)4 * nn + (seq_t)6;
    return num / (seq_t)11;
}
