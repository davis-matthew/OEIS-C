/*
 * A326368  Number of tilings of an equilateral triangle of side length n with
 *          unit triangles (of side length 1) and exactly three unit
 *          "lozenges" or "diamonds" (also of side length 1).
 * https://oeis.org/A326368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/16)*(n-2)*(9*n^5 - 9*n^4 - 81*n^3 + 81*n^2 + 160*n -
 *            192) for n >= 2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t n_minus_2 = nn - 2;

    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */
    seq_t n5 = n4 * nn;          /* n^5 */

    seq_t poly = (seq_t)9 * n5;
    poly -= (seq_t)9 * n4;
    poly -= (seq_t)81 * n3;
    poly += (seq_t)81 * n2;
    poly += (seq_t)160 * nn;
    poly -= (seq_t)192;

    return (n_minus_2 * poly) / 16;
}
