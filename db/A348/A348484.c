/*
 * A348484  Maximum number of squares on an n X n chessboard such that no two
 *          are two steps apart horizontally or vertically.
 * https://oeis.org/A348484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n^2/2 for n % 4 == 0, * (n^2 + 1)/2 for n % 4 == 1 or 3,
 *            * n^2/2 + 2 for n % 4 == 2
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;
    switch (n & 3U)
    {
        case 0: return sq / 2;          /* n % 4 == 0 */
        case 2: return sq / 2 + 2;      /* n % 4 == 2 */
        default: return (sq + 1) / 2;   /* n % 4 == 1 or 3 */
    }
}
