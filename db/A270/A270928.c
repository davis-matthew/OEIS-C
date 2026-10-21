/*
 * A270928  Number of ways to write n = x*(x-1)/2 + y*(y-1)/2 + z*(z-1)/2,
 *          where 0 < x <= y <= z, and one of x, y, z is prime.
 * https://oeis.org/A270928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x,y,z) with 0 < x <= y <= z, x*(x-1)/2
 *            + y*(y-1)/2 + z*(z-1)/2 = n, and at least one of x, y, z is
 *            prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/*  64‑bit integer floor square root.
    Only used for values small enough to fit in idx_t.  */
static idx_t _isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    seq_t cnt = 0;

    for (idx_t x = 1;; ++x) {
        unsigned __int128 tx128 = (unsigned __int128)x * (x - 1) / 2;
        if (tx128 > n)
            break;
        idx_t Tx = (idx_t)tx128;

        for (idx_t y = x;; ++y) {
            unsigned __int128 ty128 = (unsigned __int128)y * (y - 1) / 2;
            if (ty128 > n)
                break;
            idx_t Ty = (idx_t)ty128;

            idx_t sumXY = Tx + Ty;
            if (sumXY > n)
                break;

            idx_t r = n - sumXY;              /* remainder for T_z   */

            /* z must be at least y */
            if (r < Ty)
                continue;

            /* 8*r + 1 must be a perfect square */
            idx_t d = 1 + 8 * r;
            idx_t sqrt_d = _isqrt_u64(d);
            if ((unsigned __int128)sqrt_d * sqrt_d != d)
                continue;

            /* Check that z = (1 + sqrt_d)/2 is integer */
            if ((sqrt_d + 1) & 1U)
                continue;

            idx_t z = (sqrt_d + 1) / 2;
            if (z < y)
                continue;

            /* At least one of x,y,z is prime */
            if (is_prime(x) || is_prime(y) || is_prime(z))
                ++cnt;
        }
    }
    return cnt;
}
