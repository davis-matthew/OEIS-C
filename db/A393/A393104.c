/*
 * A393104  a(n) is the minimum total surface area of five-element sets of
 *          distinct integer-sided cuboids that fill an n X n X n cube.
 * https://oeis.org/A393104
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 8*n*n + 2*n + 4 for n >= 6; special values: a(3)=92,
 *            a(4)=144, a(5)=218
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    if (n == 3) return 92;
    if (n == 4) return 144;
    if (n == 5) return 218;
    seq_t nn = (seq_t)n;
    return 8 * nn * nn + 2 * nn + 4;
}
