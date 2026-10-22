/*
 * A397086  Partial sums of A397085.
 * https://oeis.org/A397086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - floor(sqrt(n))^2) mod 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of a 64‑bit unsigned value.
   Returns the largest m with m*m <= n. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1ULL << 32;      /* 2^32 > sqrt(2^64-1) */

    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * (__uint128_t)mid;

        if (sq == n)
            return mid;
        if (sq < n)
            lo = mid + 1;
        else
        {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    idx_t m   = floor_sqrt_u64(n);
    idx_t sq  = (idx_t)((__uint128_t)m * m);
    idx_t diff = n - sq;      /* 0 <= diff < 2*m + 1 < 2^33 */
    return (seq_t)(diff & 1); /* 1 if diff odd, else 0 */
}
