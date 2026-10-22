/*
 * A373584  a(n) is equal to the number of shaded cells in a regular hexagon
 *          with side n drawn on a hexagonal grid.
 * https://oeis.org/A373584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*ceiling(n*(n - 1)/4) + 1
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset protection, not used */
    seq_t nn = (seq_t)n;
    seq_t t   = nn * (nn - 1);             /* n*(n-1)  */
    seq_t ceil = (t + 3) / 4;             /* ceiling division by 4 */
    return 6 * ceil + 1;
}
