/*
 * A363711  Number of ways to write n as sum of a positive square and a
 *          positive fourth power.
 * https://oeis.org/A363711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ways to write n as a sum of a positive square
 *            and a positive fourth power
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned values.
   Returns floor(sqrt(x)). Only +, -, *, /, %, <<, >> and comparisons are used. */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;          /* sqrt(2^64-1) < 2^32 */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;             /* mid <= 2^32, mid*mid fits in idx_t */
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t t = 1;; ++t) {
        /* Compute t^4 without overflowing: t^4 = (t^2)^2, and t <= 65535 for n <= 2^64-1 */
        idx_t t2 = t * t;
        idx_t t4 = t2 * t2;
        if (t4 >= n) break;               /* t^4 must be strictly less than n */
        idx_t rem = n - t4;                /* n - t^4 > 0 since t^4 < n */
        idx_t s = isqrt_u64(rem);          /* floor sqrt of the remainder */
        if (s * s == rem)                  /* check if remainder is a perfect square */
            ++count;
    }
    return count;
}
