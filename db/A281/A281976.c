/*
 * A281976  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x,y,z,w
 *          nonnegative integers and z <= w such that both x and x + 24*y are
 *          squares.
 * https://oeis.org/A281976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (x,y,z,w) of non‑negative * integers
 *            such that * n = x^2 + y^2 + z^2 + w^2, * z ≤ w, * x is a perfect
 *            square, and x + 24·y is also a perfect square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* 64‑bit integer square root using binary search in unsigned __int128.
 * Returns floor(sqrt(n)). */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* avoid underflow */
            high = mid - 1;
        }
    }
    return high;
}

/* Count unordered pairs (z,w), z ≤ w, with z^2 + w^2 = sum. */
static seq_t count_pairs(idx_t sum)
{
    seq_t cnt = 0;
    idx_t max_z = isqrt_u64(sum);
    for (idx_t z = 0; z <= max_z; ++z) {
        unsigned __int128 z2 = (unsigned __int128)z * z;
        if (z2 > sum) break;
        idx_t rem = sum - (idx_t)z2;
        idx_t w = isqrt_u64(rem);
        if ((unsigned __int128)w * w == rem && w >= z)
            ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t total = 0;

    for (idx_t s = 0; ; ++s) {
        unsigned __int128 s2 = (unsigned __int128)s * s;
        unsigned __int128 s4 = s2 * s2;
        if (s4 > n) break;                 /* a = s^2, a^4 > n  */

        idx_t a4 = (idx_t)s4;
        idx_t rem_after_x = n - a4;

        for (idx_t t = s; ; ++t) {
            unsigned __int128 t2 = (unsigned __int128)t * t;
            unsigned __int128 diff = t2 - s2;          /* t^2 - s^2 */

            if (diff % 24u != 0) continue;

            idx_t y = (idx_t)(diff / 24u);
            unsigned __int128 y2 = (unsigned __int128)y * y;
            if (y2 > rem_after_x) break;           /* further t only increases y^2 */

            idx_t rem = rem_after_x - (idx_t)y2;
            seq_t cnt = count_pairs(rem);
            total += cnt;
        }
    }

    return total;
}
