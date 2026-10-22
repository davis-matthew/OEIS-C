/*
 * A329116  Successively count to (-1)^(n+1)*n (n = 0, 1, 2, ... ).
 * https://oeis.org/A329116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, look
 * Formula:   a(n) = (-1)^t * (t^2 - t - n) where t = ceil(sqrt(n))
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root (floor) – binary search, safe against overflow. */
static idx_t isqrt_floor(idx_t n)
{
    if (n == 0) return 0;
    idx_t lo = 0;
    idx_t hi = 0xFFFFFFFFULL;          /* sqrt(ULLONG_MAX) */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Ceil square root: ceil(sqrt(n)). */
static idx_t ceil_sqrt(idx_t n)
{
    idx_t r = isqrt_floor(n);
    if ((__uint128_t)r * r < n) r++;
    return r;
}

/* Sequence a(n). */
seq_t a(idx_t n)
{
    idx_t t = ceil_sqrt(n);
    seq_t sign = (t & 1ULL) ? -1 : 1;
    __int128 temp = (__int128)t * t - (__int128)t - (__int128)n;
    return sign * (seq_t)temp;
}
