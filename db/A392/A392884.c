/*
 * A392884  a(n) is the minimum total surface area of four-element sets of
 *          distinct integer-sided cuboids that fill an n X n X n cube.
 * https://oeis.org/A392884
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 + 2*n + 2
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 8 * nn * nn + 2 * nn + 2;
}
