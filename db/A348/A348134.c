/*
 * A348134  Number of ways two L-tiles (with rotation) can be placed on an n X
 *          n square.
 * https://oeis.org/A348134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n - 2)*(4*n^3 - 8*n^2 - 19*n + 32) for n > 1; a(1) =
 *            a(2) = 0
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;
    seq_t nn = (seq_t)n;
    seq_t term = 4 * nn * nn * nn - 8 * nn * nn - 19 * nn + 32;
    return 2 * (nn - 2) * term;
}
