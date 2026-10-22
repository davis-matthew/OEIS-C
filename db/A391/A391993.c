/*
 * A391993  Expansion of (2*x*(x - 7))/(x - 1)^3.
 * https://oeis.org/A391993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^2 + 8*n
 * Range:     u8 n <= 5, u16 n <= 103, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    return (seq_t)6 * m * m + (seq_t)8 * m;
}
