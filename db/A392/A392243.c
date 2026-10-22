/*
 * A392243  a(n) = Sum_{i=1..n} i*(-1)^ceiling(sqrt(i)).
 * https://oeis.org/A392243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^k * ((k*(k+1))^2 - n*(n+1)) / 2 where k =
 *            floor(sqrt(n))
 * Range:     i8 n <= 33, i16 n <= 1023, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    idx_t k = _floor_isqrt_u64(n);
    __int128 kk = k;
    __int128 prod = kk * (kk + 1);        /* k*(k+1) */
    __int128 sq = prod * prod;           /* (k*(k+1))^2 */
    __int128 nn = n;
    __int128 sub = nn * (nn + 1);        /* n*(n+1) */
    __int128 diff = sq - sub;
    __int128 val = diff / 2;             /* divide by 2 */
    if (k & 1) {                         /* (-1)^k */
        val = -val;
    }
    return (seq_t)val;
}
