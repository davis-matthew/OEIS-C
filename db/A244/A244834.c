/*
 * A244834  Number of length 3 0..n arrays with each partial sum starting from
 *          the beginning no more than two standard deviations from its mean.
 * https://oeis.org/A244834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   ceil_div_by2, floor_div_by2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ceil_div_by2.h"
#include "../utils/floor_div_by2.h"

/* Algorithm:
   For an integer n ≥ 1 count all 0‑≤a,b,c≤n such that
   for k = 2 and k = 3
        (2*sum_k - k*n)^2 ≤ k*n*(n+2)/3 * 4      (integer form)
   (k = 1 imposes no restriction).
   The arrays are counted by iterating over all possible sums
   S₂ = a + b that satisfy the k = 2 condition, then
   counting the admissible values of c (S₃ = S₂ + c) that satisfy the
   k = 3 condition.  The number of (a,b) pairs with sum S₂ is
   min(S₂, 2*n‑S₂) + 1.  The admissible range for c is obtained
   from (2c + K)² ≤ 4*n*(n+2) where K = 2*S₂ - 3*n.  This yields
   integer bounds on c that are intersected with [0,n].
   Complexity: O(n).
*/

/* integer square root: floor(sqrt(x))  */
static inline uint64_t floor_sqrt(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;   /* sqrt(2^64-1) < 2^32 */
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        __uint128_t prod = (__uint128_t)mid * mid;
        if (prod == (__uint128_t)x)
            return mid;
        if (prod < (__uint128_t)x)
            lo = mid + 1;
        else {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Sequenced function required by the build. */
seq_t a(idx_t n)
{
    /* 64‑bit unsigned integer for intermediate calculations.   */
    uint64_t N = (uint64_t)n;
    uint64_t twoN = 2U * N;

    /* RHS for the k = 2 inequality: 8*n*(n+2)  (fits in 64 bit for n≤1e10) */
    __uint128_t rhs2 = (__uint128_t)8U * N * (N + 2U);

    /* sqrtR = floor(sqrt(4*n*(n+2))) for the k = 3 inequality. */
    uint64_t val4 = 4U * N * (N + 2U);
    uint64_t sqrtR = floor_sqrt(val4);

    __uint128_t total = 0;

    for (uint64_t S2 = 0; S2 <= twoN; ++S2) {
        /* k = 2 condition: 3*(2*S2 - 2*n)^2 <= 8*n*(n+2) */
        int64_t t = (int64_t)(2 * S2) - (int64_t)(2 * N);
        __uint128_t lhs2 = (__uint128_t)3U * (__uint128_t)((__int128_t)t * t);
        if (lhs2 > rhs2)
            continue;

        /* number of (a,b) pairs with sum S2 */
        uint64_t minS = (S2 <= twoN - S2) ? S2 : twoN - S2;
        uint64_t countAB = minS + 1U;

        /* k = 3 condition: (2c + K)^2 <= 4*n*(n+2),  K = 2*S2 - 3*n */
        int64_t K = (int64_t)(2 * S2) - (int64_t)(3 * N);
        int64_t minC = ceil_div_by2(-(int64_t)sqrtR - K);
        int64_t maxC = floor_div_by2((int64_t)sqrtR - K);

        /* restrict to 0 ≤ c ≤ n */
        int64_t low  = (minC > 0)  ? minC : 0;
        int64_t high = (maxC < (int64_t)N) ? maxC : (int64_t)N;
        if (high >= low) {
            uint64_t countC = (uint64_t)(high - low + 1);
            total += (__uint128_t)countAB * countC;
        }
    }

    return (seq_t)total;
}
