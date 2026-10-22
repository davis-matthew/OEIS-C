/*
 * A353060  Solution to Snake Numbers Problems from 1 to n for an n X n square
 *          grid with chess knight moves (see Comments).
 * https://oeis.org/A353060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n^2 for even n≥6; a(n) = n^2-10 for odd n≥5; special
 *            cases a(1)=1, a(2)=1, a(3)=3, a(4)=13
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    if (n == 2)
        return 1;
    if (n == 3)
        return 3;
    if (n == 4)
        return 13;
    seq_t sq = (seq_t)n * (seq_t)n;
    if ((n & 1) == 0)
        return sq;
    else
        return sq - (seq_t)10;
}
