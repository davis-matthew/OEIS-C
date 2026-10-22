/*
 * A391766  Number of tetrahedra in the n X n black bishop graph.
 * https://oeis.org/A391766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3)*(n-1)*(-5 + 5*(-1)^n + 22*n - 10*(-1)^n*n - 14*n^2
 *            + 4*n^3) / 240
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses signed __int128 for safe intermediate arithmetic. */
#include <stdint.h>

seq_t a(idx_t n) {
    signed __int128 nn = (signed __int128)n;
    signed __int128 n_minus_3 = nn - 3;
    signed __int128 n_minus_1 = nn - 1;

    int parity = (n & 1) ? -1 : 1;   /* (-1)^n */
    signed __int128 D = -5
                      + (signed __int128)5 * parity
                      + (signed __int128)22 * nn
                      - (signed __int128)10 * parity * nn
                      - (signed __int128)14 * nn * nn
                      + (signed __int128)4 * nn * nn * nn;

    signed __int128 prod = n_minus_3 * n_minus_1 * D;
    signed __int128 result = prod / 240;   /* division is exact */

    return (seq_t)result;
}
