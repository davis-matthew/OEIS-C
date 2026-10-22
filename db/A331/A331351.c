/*
 * A331351  Number of achiral colorings of the edges of a cube or regular
 *          octahedron.
 * https://oeis.org/A331351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^2 + 6*n^3 + n^6 + 6*n^7 + 3*n^8) / 24
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for the intermediate calculation. */
    unsigned __int128 t = 1;
    unsigned __int128 N = (unsigned __int128)n;

    unsigned __int128 n2 = N * N;          /* n^2   */
    unsigned __int128 n3 = n2 * N;         /* n^3   */
    unsigned __int128 n6 = n3 * n3;        /* n^6   */
    unsigned __int128 n7 = n6 * N;         /* n^7   */
    unsigned __int128 n8 = n7 * N;         /* n^8   */

    unsigned __int128 sum = 8 * n2          /* 8*n^2   */
                         + 6 * n3          /* 6*n^3   */
                         + n6             /* n^6     */
                         + 6 * n7         /* 6*n^7   */
                         + 3 * n8;        /* 3*n^8   */

    sum /= 24; /* integer division */

    return (seq_t)sum;
}
