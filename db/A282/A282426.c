/*
 * A282426  Number of ways to write n as x^4 + 4*y^2 + z^2 + 3^k, where x,y,z
 *          are nonnegative integers and k is among 0,1,2,3,4.
 * https://oeis.org/A282426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { (x,y,z,k) | x,y,z >= 0, k∈{0,1,2,3,4}, n = x^4 +
 *            4*y^2 + z^2 + 3^k }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(v)) */
static inline idx_t isqrt(idx_t v)
{
    idx_t lo = 0;
    idx_t hi = v;
    idx_t res = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= v) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* 3^k for k = 0..4 */
    static const idx_t pow3[5] = {1, 3, 9, 27, 81};

    seq_t count = 0;                       /* result */

    for (int k = 0; k < 5; ++k) {
        if (n < pow3[k]) continue;          /* n < 3^k, skip */
        idx_t rem = n - pow3[k];            /* remaining after 3^k */

        /* x loop: x^4 <= rem */
        for (idx_t x = 0; ; ++x) {
            __uint128_t x2 = (__uint128_t)x * x;
            __uint128_t x4 = x2 * x2;
            if (x4 > rem) break;            /* no larger x possible */
            idx_t rem_after_x = rem - (idx_t)x4;

            /* y loop: 4*y^2 <= rem_after_x */
            for (idx_t y = 0; ; ++y) {
                __uint128_t y2 = (__uint128_t)y * y;
                __uint128_t yTerm = (__uint128_t)4 * y2;
                if (yTerm > rem_after_x) break;
                idx_t rem_after_y = rem_after_x - (idx_t)yTerm;

                /* Check if rem_after_y is a perfect square */
                idx_t z = isqrt(rem_after_y);
                if ((__uint128_t)z * z == rem_after_y) {
                    ++count;                /* found a valid quadruple */
                }
            }
        }
    }
    return count;
}
