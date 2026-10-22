/*
 * A340631  a(n) is the minimum number of pebbles such that any assignment of
 *          those pebbles on a complete graph with n vertices is a next-player
 *          winning game in the two-player impartial pebbling game.
 * https://oeis.org/A340631
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3)=7, a(4)=23; For n >= 5: if n is odd, a(n) = n + 2; if n is
 *            even, a(n) = n + 7; This implements the closed formulas given in
 *            the OEIS entry
 * Range:     u8 n <= 249, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3)
        return (seq_t)7;
    if (n == 4)
        return (seq_t)23;
    if (n % 2 == 1)  /* odd n >= 5 */
        return (seq_t)n + 2;
    else            /* even n >= 6 */
        return (seq_t)n + 7;
}
