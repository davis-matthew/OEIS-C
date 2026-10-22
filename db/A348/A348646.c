/*
 * A348646  a(n) = (72*n + 5)*(1296*n^2 + 153*n + 4).
 * https://oeis.org/A348646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (72*n + 5)*(1296*n^2 + 153*n + 4)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 35, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t part1 = (seq_t)72 * nn + (seq_t)5;
    seq_t part2 = (seq_t)1296 * nn * nn + (seq_t)153 * nn + (seq_t)4;
    return part1 * part2;
}
