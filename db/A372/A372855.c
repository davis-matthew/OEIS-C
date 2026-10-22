/*
 * A372855  Number of ways two dihexes can be placed on an n-th regular
 *          hexagonal board.
 * https://oeis.org/A372855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/2)*(27*n^4 - 90*n^3 + 78*n^2 + 11*n - 24) for n > 1
 *            a(1) = 0
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 101, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */

    seq_t term = 27 * n4 - 90 * n3 + 78 * n2 + 11 * nn - 24;
    /* term is even for all n > 1, so the division is exact */
    return (term * 3) / 2;
}
