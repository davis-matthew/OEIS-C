/*
 * A262136  Number of distinct fractional parts of the numbers Sum_{i=j..k}
 *          (-1)^i/i with 1 <= j <= k <= n, where the fractional part of x is
 *          given by x - floor(x).
 * https://oeis.org/A262136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)/2 + 2 - floor((sqrt(4 n+1)-1)/2) - floor((sqrt(4
 *            n+1)-1)/4) with a(1) = 1 (formula holds for all n > 1)
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        /* avoid overflow: mid*mid <= x  <=>  mid <= x/mid  (mid!=0) */
        if (mid == 0 || mid <= x / mid)
        {
            ans = mid;
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;  /* a(1) = 1 */

    seq_t nn = (seq_t)n;
    /* n*(n-1)/2 + 2 */
    seq_t base = nn * (nn - 1) / 2 + 2;

    /* compute floor((sqrt(4*n + 1)-1)/2) and floor((sqrt(4*n + 1)-1)/4) */
    idx_t sq = floor_sqrt_u64(4 * n + 1);
    seq_t f2 = (seq_t)((sq - 1) / 2);
    seq_t f4 = (seq_t)((sq - 1) / 4);

    return base - f2 - f4;
}
