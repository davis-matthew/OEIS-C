/*
 * A282513  a(n) = floor((3*n + 2)^2/24 + 1/3).
 * https://oeis.org/A282513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3*n + 2)^2 / 24 + 1/3)
 * Range:     u8 n <= 4, u16 n <= 170, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = 3 * nn + 2;
    seq_t prod = t * t + 8;
    return prod / 24;
}
