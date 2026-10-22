/*
 * A302559  Partial sums of A092183.
 * https://oeis.org/A302559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(584 - 105*n - 2120*n^2 + 135*n^3 + 1566*n^4)/60
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 43, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 128‑bit signed integer to avoid intermediate overflow. */
    __int128 nn = n;
    __int128 p  = 1566;           /* 1566 */
    p = p * nn + 135;             /* 1566*n + 135 */
    p = p * nn - 2120;            /* 1566*n^2 + 135*n - 2120 */
    p = p * nn - 105;             /* 1566*n^3 + 135*n^2 - 2120*n - 105 */
    p = p * nn + 584;             /* 1566*n^4 + 135*n^3 - 2120*n^2 - 105*n + 584 */
    __int128 res = nn * p;        /* n * P(n) */
    res /= 60;                    /* divide by 60 */
    return (seq_t)res;            /* cast to the sequence type */
}
