/*
 * A285188  a(n) = Sum_{k=1..n} (k^2*floor(k/2)).
 * https://oeis.org/A285188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(3*n^2 + n - 1 + 3*(-1)^n)/24
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 430, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    unsigned __int128 N = (unsigned __int128)n;
    /* compute 3*n^2 + n - 1 + 3*(-1)^n */
    unsigned __int128 internal = 3 * N * N + N - 1;
    if (n & 1)          /* odd n → add (-3) */
        internal -= 3;
    else                /* even n → add (+3) */
        internal += 3;

    unsigned __int128 num = N * (N + 1) * internal;
    unsigned __int128 res = num / 24;
    return (seq_t)res;
}
