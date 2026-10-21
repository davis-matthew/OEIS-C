/*
 * A283114  Number of nonequivalent ways (mod D_3) to place 3 points on an n X
 *          n X n triangular grid so that no two of them are on the same row,
 *          column or diagonal.
 * https://oeis.org/A283114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 - 9*n^5 + 27*n^4 - 20*n^3 - 24*n^2 + 24*n)/288 +
 *            IF(n mod 2 == 1, (n^2 - 3*n - 5)/32, 0) + IF(n mod 3 == 1, 2/9,
 *            0) (All arithmetic performed in 128‑bit signed integers then
 *            cast to seq_t)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 105, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use signed 128–bit for intermediate calculations */
    __int128 N   = (__int128)n;
    __int128 n2  = N * N;
    __int128 n3  = n2 * N;
    __int128 n4  = n3 * N;
    __int128 n5  = n4 * N;
    __int128 n6  = n5 * N;

    /* numerator over the common denominator 288 */
    __int128 num =
          n6
        - (__int128)9  * n5
        + (__int128)27 * n4
        - (__int128)20 * n3
        - (__int128)24 * n2
        + (__int128)24 * N;

    /* add the odd‑n part (9*(n^2-3*n-5)) */
    if (n & 1) {
        __int128 odd_val = n2 - (__int128)3 * N - 5;
        num += (__int128)9 * odd_val;
    }

    /* add the n mod 3 == 1 part (64) */
    if ((n % 3) == 1) {
        num += 64;           /* 2/9 * 288 = 64 */
    }

    /* divide by 288 to get the integer result */
    seq_t result = (seq_t)(num / 288);
    return result;
}
