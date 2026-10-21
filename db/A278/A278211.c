/*
 * A278211  Maximum total number of possible moves that any number of rooks of
 *          the same color can make on an n X n chessboard.
 * https://oeis.org/A278211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n*n - 16*n + 24 for n >= 7; Special cases: a(1)=0,
 *            a(2)=4, a(3)=12, a(4)=28, a(5)=48, a(6)=76, a(7)=108
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, but guard */
    switch (n)
    {
        case 1: return 0;
        case 2: return 4;
        case 3: return 12;
        case 4: return 28;
        case 5: return 48;
        case 6: return 76;
        case 7: return 108;
        default:
        {
            seq_t N = (seq_t)n;
            return 4*N*N - 16*N + 24;
        }
    }
}
