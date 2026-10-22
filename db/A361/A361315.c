/*
 * A361315  a(n) is the minimum number of pebbles such that any assignment of
 *          those pebbles on a complete graph with n vertices is a next-player
 *          winning game in the two-player impartial (3;1,1) pebbling game.
 * https://oeis.org/A361315
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2n + 1 for n >= 8; a(4)=31, a(5)=26, a(6)=19, a(7)=17,
 *            a(8)=17
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 0‑based index; for the OEIS offset a(4) is the first term.  */
    if (n < 4) return 0;                /* behavior below the offset is irrelevant */
    if (n == 4) return 31;               /* a(4) */
    if (n == 5) return 26;               /* a(5) */
    if (n == 6) return 19;               /* a(6) */
    if (n == 7) return 17;               /* a(7) */
    if (n == 8) return 17;               /* a(8) */
    /* for n >= 8: a(n) = 2*n + 1 */
    return (seq_t) n * 2 + 1;
}
