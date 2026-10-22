/*
 * A385602  a(n) is the minimum number of squares from which an n-fold totally
 *          concave polyomino (n-TCP) can be made.
 * https://oeis.org/A385602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(n+1)^2 - 1 for n > 2, with a(1) = 21, a(2) = 50
 * Range:     u8 n <= 5, u16 n <= 103, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 21;
    if (n == 2) return 50;
    seq_t nn = (seq_t)n + 1;
    return 6 * nn * nn - 1;
}
