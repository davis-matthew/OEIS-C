/*
 * A346197  a(n) is the minimum number of pebbles such that any assignment of
 *          those pebbles on K_5 is a next-player winning game in the
 *          two-player impartial (n+1,n) pebbling game.
 * https://oeis.org/A346197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 6*n + 3 for 2 <= n <= 6, a(1)=7, a(n) = 6*n + 5 +
 *            2*floor((n-7)/3) for n >= 7
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Optional guard for n == 0 (out of range) */
    if (n == 0)
        return 0;

    if (n == 1)
        return (seq_t)7;

    if (n <= 6)
        return (seq_t)n * 6 + 3;

    /* n >= 7 */
    return (seq_t)n * 6 + 5 + (seq_t)2 * ((n - 7) / 3);
}
