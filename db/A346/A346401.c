/*
 * A346401  a(n) is the minimum number of pebbles such that any assignment of
 *          those pebbles on a complete graph with n vertices is a next-player
 *          winning game in the two-player impartial (3, 2) pebbling game.
 * https://oeis.org/A346401
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 13 for n=3, * a(n) = 21 for n=4, * a(n) = 15 for n=5, *
 *            a(n) = 2n+9 for even n >= 6, * a(n) = 2n+3 for odd n >= 7
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the exceptional small cases. */
    if (n == 3) return (seq_t)13;
    if (n == 4) return (seq_t)21;
    if (n == 5) return (seq_t)15;

    /* General formula for n >= 6. */
    if ((n & 1u) == 0)  /* even n */
        return ((seq_t)n * (seq_t)2u) + (seq_t)9;
    else                /* odd n */
        return ((seq_t)n * (seq_t)2u) + (seq_t)3;
}
