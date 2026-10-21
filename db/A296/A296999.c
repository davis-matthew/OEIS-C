/*
 * A296999  Number of nonequivalent (mod D_8) ways to place 4 points on an n X
 *          n point grid so that no point is equally distant from two other
 *          points on the same row or the same column.
 * https://oeis.org/A296999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 - 6*n^6 - 12*n^5 + 64*n^4 + 8*n^3 - 136*n^2 + (n
 *            odd)*(14*n^4 - 96*n^3 + 162*n^2 - 92*n + 93))/192 + (n%6==2)*n/6
 *            + (n%4==2)*n/4 + (n%6==5)*(n+1)/6 The implementation rewrites
 *            the expression with a common denominator 192: a(n) = ((n^8 -
 *            6*n^6 - 12*n^5 + 64*n^4 + 8*n^3 - 136*n^2 + (n o...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 30, u64 n <= 493, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use signed 128‑bit intermediate to avoid underflow. */
    __int128 N = ( __int128 ) n;
    __int128 n2 = N * N;          /* n^2 */
    __int128 n3 = n2 * N;         /* n^3 */
    __int128 n4 = n2 * n2;        /* n^4 */
    __int128 n5 = n4 * N;         /* n^5 */
    __int128 n6 = n3 * n3;        /* n^6 */
    __int128 n8 = n4 * n4;        /* n^8 */

    __int128 num = n8
                   - 6 * n6
                   - 12 * n5
                   + 64 * n4
                   + 8  * n3
                   - 136 * n2;

    /* odd‑n correction */
    if ( (n & 1) != 0 ) {
        num += 14 * n4
               - 96 * n3
               + 162 * n2
               - 92 * N
               + 93;
    }

    /* modulus corrections with common denominator 192 */
    if ( n % 6 == 2 ) num += 32 * N;
    if ( n % 4 == 2 ) num += 48 * N;
    if ( n % 6 == 5 ) num += 32 * (N + 1);

    /* final division, the numerator is always an exact multiple of 192 */
    __int128 res = num / 192;

    return (seq_t) res;
}
