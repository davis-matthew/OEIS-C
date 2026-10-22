/*
 * A338139  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x^2 +
 *          26*y^2 - 11*x*y a power of two (including 2^0 = 1), where x, y, z,
 *          w are nonnegative integers with z <= w.
 * https://oeis.org/A338139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑negative integer quadruples (x,y,z,w) with
 *            z <= w such that x^2 + y^2 + z^2 + w^2 = n and x^2 + 26*y^2 -
 *            11*x*y is a power of two
 * Range:     u8 n <= 28, u16 n <= 4679, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPowerOfTwo
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPowerOfTwo.h"

/* Integer square root (floor), suitable for idx_t values up to 10^12. */
static idx_t isqrt_u64(idx_t v)
{
    idx_t low = 0, high = v;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t mid2 = mid * mid;
        if (mid2 == v) return mid;
        if (mid2 < v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Count unordered pairs (z,w), z <= w, with z^2 + w^2 = s. */
static seq_t count_pairs(idx_t s)
{
    seq_t cnt = 0;
    for (idx_t z = 0; z * z <= s; ++z) {
        idx_t rem = s - z * z;
        idx_t w = isqrt_u64(rem);
        if (w * w == rem && w >= z)
            cnt += 1;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* sequence offset is 1; n>=1 in normal use */
    seq_t total = 0;
    for (idx_t x = 0; x * x <= n; ++x) {
        idx_t x2 = x * x;
        for (idx_t y = 0; y * y <= n - x2; ++y) {
            idx_t y2 = y * y;
            seq_t expr = (seq_t)x2 + (seq_t)26 * (seq_t)y2
                         - (seq_t)11 * (seq_t)x * (seq_t)y;
            if (expr > 0 && isPowerOfTwo(expr)) {
                idx_t s = n - x2 - y2;
                total += count_pairs(s);
            }
        }
    }
    return total;
}
