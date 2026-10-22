/*
 * A394192  a(n) = n^3 - n^2 + 5*n + 2.
 * https://oeis.org/A394192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*n*n - n*n + 5*n + 2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return x * x * x - x * x + 5 * x + 2;
}
