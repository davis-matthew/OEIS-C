/*
 * A245828  Szeged index of the grid graph P_n X P_n.
 * https://oeis.org/A245828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 * (n^2 - 1) / 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 48, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    unsigned __int128 t = n;
    unsigned __int128 n2 = t * t;          /* n^2       */
    unsigned __int128 n4 = n2 * n2;        /* n^4       */
    unsigned __int128 prod = n4 * (n2 - 1);/* n^4*(n^2-1) */
    prod /= 3;                            /* exact division */

    return (seq_t)prod;
}
