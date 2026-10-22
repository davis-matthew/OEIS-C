/*
 * A375003  Number of ordered solutions (x,y,z,w) to x*y + x*z + x*w + y*z +
 *          y*w + z*w = n with x,y,z,w >= 1.
 * https://oeis.org/A375003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered positive integer solutions (x,y,z,w) *
 *            to x*y + x*z + x*w + y*z + y*w + z*w = n. The function
 *            enumerates all * pairs (x,y) with xy <= n, turns the remaining
 *            equation into a divisor * counting problem for M = (n-xy) +
 *            (x+y)^2, and adds the number of ordered * factor pairs...
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square root for 64‑bit values using binary search and __uint128_t
 * to avoid overflow.  The function returns floor(sqrt(x)). */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t low = 0;
    uint64_t high = x;
    while (low <= high)
    {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Count ordered factor pairs (u,v) such that
 * u*v == M  and  u,v >= min_factor.  For each unordered pair
 * it contributes 1 if u==v otherwise 2. */
static inline seq_t count_factor_pairs_uint64(uint64_t M, uint64_t min_factor)
{
    seq_t res = 0;
    uint64_t sqrtM = isqrt_uint64(M);
    for (uint64_t u = min_factor; u <= sqrtM; ++u)
    {
        if (M % u == 0)
        {
            uint64_t v = M / u;
            if (v < min_factor)
                continue;
            if (u == v)
                res += 1;
            else
                res += 2;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* For n < 6 no solution exists, but the following loop already
     * yields 0 in that case, so no special handling is required. */
    seq_t total = 0;

    /* Iterate over all ordered pairs (x,y) with xy <= n. */
    for (idx_t x = 1; x <= n; ++x)
    {
        idx_t max_y = n / x;          /* xy <= n   */
        if (max_y == 0)
            break;                    /* further x will give no valid y */
        for (idx_t y = 1; y <= max_y; ++y)
        {
            uint64_t xy = (uint64_t)x * (uint64_t)y;
            uint64_t K  = (uint64_t)n - xy;          /* n - xy >= 0   */
            uint64_t d  = (uint64_t)x + (uint64_t)y; /* x+y          */
            uint64_t M  = K + d * d;               /* (n-xy)+(x+y)^2 */
            if (M == 0)                            /* should not happen when K>=0, but guard */
                continue;
            uint64_t min_factor = d + 1;           /* u,v >= d+1   */
            total += count_factor_pairs_uint64(M, min_factor);
        }
    }
    return total;
}
