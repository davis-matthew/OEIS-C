/*
 * A317984  Expansion of g.f. 140*x*(1 + 4*x + x^2) / (1 - x)^5.
 * https://oeis.org/A317984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*n^4 + 70*n^3 + 35*n^2 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 104, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;
    seq_t x2 = x * x;          /* n^2 */
    seq_t x3 = x2 * x;         /* n^3 */
    seq_t x4 = x3 * x;         /* n^4 */
    return 35 * x4 + 70 * x3 + 35 * x2;
}
