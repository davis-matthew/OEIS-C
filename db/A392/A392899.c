/*
 * A392899  Number of bipartite complete simple games with a maximum number of
 *          minimal winning pairs.
 * https://oeis.org/A392899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 14*n - 24)/8 if n even, (n-1)/2 if n odd
 * Range:     u8 n <= 39, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1, offset 1 */
    if ((n & 1) == 0) {
        idx_t val = n * (n + 14) - 24;
        return (seq_t)(val / 8);
    } else {
        return (seq_t)((n - 1) / 2);
    }
}
