/*
 * A288349  Partial sums of A059268.
 * https://oeis.org/A288349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^t + 2^(n - t*(t-1)/2) - t - 2, where t =
 *            floor(sqrt(2*n)+1/2), n>=1
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* Integer square root: floor(sqrt(x)) for 0 <= x < 2^64  */
static idx_t floor_isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = ((idx_t)1 << 32);   // sqrt(2^64-1) < 2^32
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* t = floor( sqrt(2*n) + 1/2 )  */
    idx_t x = n + n; /* 2*n, safe for the ranges we need */
    idx_t r = floor_isqrt_u64(x);         /* floor(sqrt(2*n))      */
    unsigned __int128 left  = (unsigned __int128)4 * x;                 /* 4*2*n = 8*n          */
    unsigned __int128 right = (unsigned __int128)4 * r * r
                            + (unsigned __int128)4 * r + 1;            /* 4*r^2 + 4*r + 1     */
    idx_t t = (left >= right) ? r + 1 : r;

    /* compute n - t*(t-1)/2  */
    unsigned __int128 prod = (unsigned __int128)t * (t - 1) / 2;
    idx_t sub = n - (idx_t)prod;

    /* calculate 2^t and 2^sub using pow2 utility */
    seq_t val1 = pow2(t);
    seq_t val2 = pow2(sub);

    seq_t val = val1 + val2;
    val -= (seq_t)t;
    val -= (seq_t)2;

    return val;
}
