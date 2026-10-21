/*
 * A246739  Number of length 2+4 0..n arrays with no pair in any consecutive
 *          five terms totalling exactly n.
 * https://oeis.org/A246739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = even n: -58*n + 111*n^2 - 87*n^3 + 36*n^4 - 8*n^5 + n^6
 *            odd n: -70 + 80*n + 34*n^2 - 71*n^3 + 36*n^4 - 8*n^5 + n^6
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 41, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* behaviour below offset is irrelevant */

    /* Use a 128‑bit signed intermediate.  Works in both C and C++. */
    __int128 nn   = ( __int128 ) n;
    __int128 n2   = nn * nn;
    __int128 n3   = n2 * nn;
    __int128 n4   = n3 * nn;
    __int128 n5   = n4 * nn;
    __int128 n6   = n5 * nn;

    __int128 res;
    if ((n & 1) == 0) {           /* even n */
        res = -(__int128)58 * nn
              + (__int128)111 * n2
              - (__int128)87  * n3
              + (__int128)36  * n4
              - (__int128)8   * n5
              + n6;
    } else {                      /* odd n */
        res = -(__int128)70
              + (__int128)80 * nn
              + (__int128)34 * n2
              - (__int128)71 * n3
              + (__int128)36 * n4
              - (__int128)8  * n5
              + n6;
    }
    return (seq_t)res;
}
