/*
 * A302339  Triangle read by rows: T(n,k) = number of linear operators T on an
 *          n-dimensional vector space over GF(2) such that U is invariant
 *          under T for some given k-dimensional subspace U.
 * https://oeis.org/A302339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A302339: Triangle read by rows.
   T(n,k) = 2^(k^2 + n*(n-k)),
   a(m) is the flattening of T(n,k) by rows, offset 0. */
#include <stdint.h>
#include "../utils/pow2.h"

/* integer square‑root: floor(sqrt(x)) for 0 <= x < 2^64. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (x >> 1) + 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x)            return mid;
        if (sq <  x)            lo = mid + 1;
        else                    hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* row r : 0‑based, satisfies r(r+1)/2 <= n < (r+1)(r+2)/2   */
    idx_t s = isqrt_u64(8 * n + 1);      /* floor(sqrt(8*n+1))                */
    idx_t r = (s - 1) / 2;                /* row index                         */
    idx_t t = r * (r + 1) / 2;            /* number of elements before row r   */
    idx_t k = n - t;                      /* column index in row r             */

    /* exponent of 2 */
    idx_t exp = k * k + r * (r - k);

    return pow2(exp);                     /* 2^exp                            */
}
