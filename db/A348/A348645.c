/*
 * A348645  a(n) = (12*n + 1)*(5184*n^2 + 540*n + 13).
 * https://oeis.org/A348645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 1)*(5184*n^2 + 540*n + 13)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 40, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;
    seq_t part1 = 12 * n_s + 1;
    seq_t part2 = 5184 * n_s * n_s + 540 * n_s + 13;
    return part1 * part2;
}
