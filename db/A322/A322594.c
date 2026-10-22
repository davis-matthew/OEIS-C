/*
 * A322594  a(n) = (4*n^3 + 12*n^2 - 4*n + 3)/3.
 * https://oeis.org/A322594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 12*n^2 - 4*n + 3)/3
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t num = 4 * x * x * x + 12 * x * x - 4 * x + 3;
    return num / 3;
}
