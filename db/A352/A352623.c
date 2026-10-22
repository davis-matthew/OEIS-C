/*
 * A352623  a(n) is the maximum number k of queens that can be placed on an n
 *          X n chessboard such that, for each number j in 0..k-1, there is a
 *          queen attacking exactly j unoccupied squares.
 * https://oeis.org/A352623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = floor((n+1)*(n+2)/4) for n >= 6. Base cases: a(0)=0,
 *            a(1)=1, a(2)=0, a(3)=5, a(4)=8, a(5)=11
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the small exceptional values explicitly. */
    switch (n)
    {
        case 0: return 0;
        case 1: return 1;
        case 2: return 0;
        case 3: return 5;
        case 4: return 8;
        case 5: return 11;
        default:
        {
            /* For n >= 6 the formula floor((n+1)*(n+2)/4) holds. */
            seq_t N = (seq_t)n;
            seq_t prod = (N + 1) * (N + 2);
            return prod / 4;
        }
    }
}
