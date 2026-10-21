/*
 * A279448  Number of nonequivalent ways to place 4 points on an n X n square
 *          grid so that no more than 2 points are on a vertical or horizontal
 *          straight line.
 * https://oeis.org/A279448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n⁸ - 14·n⁶ + 30·n⁵ + 12·n⁴ - 60·n³ + 40·n²)/192 * + IF(n
 *            is odd, (4·n⁴ - 20·n³ + 22·n² - 2·n - 7)/64, 0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 30, u64 n <= 493, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Use a 128‑bit integer to avoid overflow for the intermediate
       computation in the 64‑bit build.  The result always fits
       into seq_t because the division guarantees an integer value. */
    unsigned __int128 N = (unsigned __int128) n;
    unsigned __int128 n2 = N * N;            /* n² */
    unsigned __int128 n3 = n2 * N;           /* n³ */
    unsigned __int128 n4 = n3 * N;           /* n⁴ */
    unsigned __int128 n5 = n4 * N;           /* n⁵ */
    unsigned __int128 n6 = n5 * N;           /* n⁶ */
    unsigned __int128 n8 = n4 * n4;          /* n⁸ */

    unsigned __int128 poly1 =
        n8
        - (unsigned __int128)14 * n6
        + (unsigned __int128)30 * n5
        + (unsigned __int128)12 * n4
        - (unsigned __int128)60 * n3
        + (unsigned __int128)40 * n2;

    if (n & 1)   /* n is odd */
    {
        unsigned __int128 poly2 =
            (unsigned __int128)4 * n4
            - (unsigned __int128)20 * n3
            + (unsigned __int128)22 * n2
            - (unsigned __int128)2 * N
            - (unsigned __int128)7;
        /* (poly1 + 3·poly2)/192 implements the two terms together */
        poly1 += 3 * poly2;
    }

    return (seq_t)(poly1 / 192);
}
