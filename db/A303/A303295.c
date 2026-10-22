/*
 * A303295  a(n) is the maximum water retention of a height-3 length-n number
 *          parallelogram with maximum water area.
 * https://oeis.org/A303295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 20, a(n) = 8*n^2 + 10*n - 3 for n > 1
 * Range:     u8 n <= 5, u16 n <= 89, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 20;
    seq_t m = (seq_t)n;
    return 8 * m * m + 10 * m - 3;
}
