/*
 * A374083  Vertical moments of inertia of a unit lozenge tiling of the
 *          hexagon with side lengths n (see references for exact
 *          configuration).
 * https://oeis.org/A374083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n^4 - n^2) / 6
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 245, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;      /* cast index to seq_t */
    seq_t n2 = nn * nn;       /* n^2 */
    seq_t n4 = n2 * n2;       /* n^4 */
    return (7 * n4 - n2) / 6;
}
