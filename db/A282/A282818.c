/*
 * A282818  Number of inequivalent ways to color the edges of a tetrahedron
 *          using at most n colors so that no two adjacent edges have the same
 *          color.
 * https://oeis.org/A282818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n^3-9*n^2+32*n-38)/12
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 3 the product contains a zero factor, so a(n) = 0. */
    if (n < 3u) return 0;

    /* Use unsigned __int128 for intermediate calculations to avoid overflow. */
    unsigned __int128 N   = n;
    unsigned __int128 N1  = N - 1;
    unsigned __int128 N2  = N - 2;

    /* First factor: n*(n-1)*(n-2) */
    unsigned __int128 f1 = N * N1 * N2;

    /* Second factor: n^3 - 9*n^2 + 32*n - 38 */
    unsigned __int128 n2     = N * N;
    unsigned __int128 n3     = n2 * N;
    unsigned __int128 poly   = n3
                             - (unsigned __int128)9 * n2
                             + (unsigned __int128)32 * N
                             - (unsigned __int128)38;

    /* Multiply the two factors and divide by 12 */
    unsigned __int128 prod = f1 * poly;
    prod /= 12;

    return (seq_t)prod;
}
