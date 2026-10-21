/*
 * A284621  Positions of 0 in A284620.
 * https://oeis.org/A284621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * floor( (3*(n-1) + sqrt(5*(n-1)^2)) / 2 ) + 1
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows the OEIS definition a(n+1) = 2*A001950(n)+1 with A001950(n)=floor( (3n + sqrt(5n^2)) / 2 ). */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer, returning the
   largest 64‑bit value x such that x*x <= v.  */
static idx_t isqrt_uint128(unsigned __int128 v)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = (unsigned __int128)~0ULL;   /* 2^64-1 */
    unsigned __int128 ans = 0;

    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid == 0) {            /* avoid division by zero below */
            ans = 0;
            low = 1;
            continue;
        }
        if (mid <= v / mid) {      /* mid*mid <= v  (avoids overflow) */
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return (idx_t)ans;
}

seq_t a(idx_t n)
{
    idx_t k = n - 1;                               /* k >= 0 for n >= 1 */

    /* Compute sqrt(5 * k * k) exactly with 128‑bit arithmetic. */
    unsigned __int128 prod = (unsigned __int128)5 * k * k;
    idx_t s = isqrt_uint128(prod);                 /* floor(sqrt(5*k^2)) */

    /* (3k + s) / 2  (floor division) */
    unsigned __int128 t = ((unsigned __int128)3 * k + s) / 2;

    /* a(n) = 2*t + 1 */
    unsigned __int128 res = 2 * t + 1;
    return (seq_t)res;
}
