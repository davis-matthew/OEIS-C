/*
 * A242279  Number of inequivalent (mod D_4) ways four checkers can be placed
 *          on an n X n board.
 * https://oeis.org/A242279
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 - 6*n^6 + 40*n^4 - 48*n^3 + 16*n^2 + (n odd)*(14*n^4
 *            - 48*n^3 + 34*n^2 - 3))/192
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 30, u64 n <= 493, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;                /* offset starts at 2 */

    /* Intermediate arithmetic in 128‑bit unsigned to avoid overflow on 64‑bit seq_t */
    unsigned long long N = (unsigned long long)n;
    __int128 n2 = (__int128)N * N;             /* n^2 */
    __int128 n3 = n2 * N;                      /* n^3 */
    __int128 n4 = n2 * n2;                     /* n^4 */
    __int128 n6 = n3 * n3;                     /* n^6 */
    __int128 n8 = n4 * n4;                     /* n^8 */

    __int128 base = n8
                    - (__int128)6 * n6
                    + (__int128)40 * n4
                    - (__int128)48 * n3
                    + (__int128)16 * n2;

    if ((n & 1ULL) != 0ULL) {                  /* n is odd */
        __int128 odd_term = (__int128)14 * n4
                            - (__int128)48 * n3
                            + (__int128)34 * n2
                            - (__int128)3;
        base += odd_term;
    }

    __int128 result = base / (__int128)192;
    return (seq_t)result;
}
