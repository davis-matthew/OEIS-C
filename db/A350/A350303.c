/*
 * A350303  a(n) is the number of ways to partition the set of vertices of a
 *          convex (n+14)-gon into 5 nonintersecting polygons.
 * https://oeis.org/A350303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+11)*(n+12)*(n+13)*(n+14) / 2880
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 9, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = nn *
                 (nn + 1U) *
                 (nn + 2U) *
                 (nn + 3U) *
                 (nn + 11U) *
                 (nn + 12U) *
                 (nn + 13U) *
                 (nn + 14U);
    return prod / (seq_t)2880U;
}
