/*
 * A254473  24-hedral numbers: a(n) = (2*n + 1)*(8*n^2 + 14*n + 7).
 * https://oeis.org/A254473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*(8*n^2+14*n+7)
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 644, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t part1 = (seq_t)2 * nn + 1;
    seq_t part2 = (seq_t)8 * nn * nn + (seq_t)14 * nn + 7;
    return part1 * part2;
}
