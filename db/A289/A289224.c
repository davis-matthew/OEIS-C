/*
 * A289224  Number of ways to select 3 disjoint point triples from an n X n X
 *          n triangular point grid, each point triple forming a 2 X 2 X 2
 *          triangle.
 * https://oeis.org/A289224
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (n^6 - 6*n^5 - 24*n^4 + 208*n^3 - 67*n^2 - 1684*n + 2712)
 *            / 6
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 41, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;
    seq_t n4 = n3 * sn;
    seq_t n5 = n4 * sn;
    seq_t n6 = n5 * sn;

    /* compute left = n^6 + 208*n^3 + 2712   */
    seq_t left = n6 + (seq_t)208 * n3 + (seq_t)2712;
    /* compute right = 6*n^5 + 24*n^4 + 67*n^2 + 1684*n */
    seq_t right = (seq_t)6 * n5 + (seq_t)24 * n4 + (seq_t)67 * n2 + (seq_t)1684 * sn;

    seq_t numerator = left - right;
    return numerator / (seq_t)6;
}
