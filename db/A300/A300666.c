/*
 * A300666  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with
 *          nonnegative integers x,y,z,w and z <= w such that x or 2*y is a
 *          square and x + 3*y is also a square.
 * https://oeis.org/A300666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = # of (x,y,z,w) ∈ ℕ⁴ with 
    x² + y² + z² + w² = n,
    z ≤ w,
    (x is a square OR 2·y is a square),
    x + 3·y is a square
*/
#include <stdint.h>
#include <stdbool.h>

/* integer square root: floor(sqrt(n)) for uint64_t */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = 1ULL << 32, mid, res = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

/* test whether v is a perfect square */
static inline bool is_square(uint64_t v)
{
    uint64_t r = isqrt_u64(v);
    return r * r == v;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    uint64_t max_val = isqrt_u64(n);

    for (uint64_t x = 0; x <= max_val; ++x) {
        uint64_t x2 = x * x;
        if (x2 > n) break;

        for (uint64_t y = 0; y <= max_val; ++y) {
            uint64_t y2 = y * y;
            uint64_t xy_sum = x2 + y2;
            if (xy_sum > n) break;

            if (!(is_square(x) || is_square(2 * y))) continue;
            if (!is_square(x + 3 * y)) continue;

            uint64_t rem = n - xy_sum;
            uint64_t z_max = isqrt_u64(rem);

            for (uint64_t z = 0; z <= z_max; ++z) {
                uint64_t z2 = z * z;
                if (z2 > rem) break;
                uint64_t rest = rem - z2;
                if (rest < z2) continue;   /* ensures w ≥ z */

                uint64_t w = isqrt_u64(rest);
                if (w * w == rest && w >= z) {
                    ++count;
                }
            }
        }
    }
    return count;
}
