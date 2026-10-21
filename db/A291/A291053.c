/*
 * A291053  Number of maximal irredundant sets in the n-Andrásfai graph.
 * https://oeis.org/A291053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-3120 + 10932*n - 4970*n^2 + 765*n^3 - 10*n^4 +
 *            3*n^5)/120 (n > 3) a(1)=2, a(2)=10, a(3)=28
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 2;
    if (n == 2) return 10;
    if (n == 3) return 28;

    __int128 nn = n;
    __int128 v  = 3;
    v = v * nn - 10;
    v = v * nn + 765;
    v = v * nn - 4970;
    v = v * nn + 10932;
    v = v * nn - 3120;
    v /= 120;
    return (seq_t)v;
}
