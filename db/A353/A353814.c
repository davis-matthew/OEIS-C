/*
 * A353814  a(n) = 1 if n is the sum of 2 distinct nonzero squares, 0
 *          otherwise.
 * https://oeis.org/A353814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is the sum of 2 distinct nonzero squares, 0
 *            otherwise
 * Range:     u8 n <= 8106, u16 n <= 8106, u32 n <= 8106, u64 n <= 8106,
 *            u128 n <= 8106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for uint64_t values, floor(sqrt(x)) */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low  = 0;
    idx_t high = (idx_t)1ULL << 32;          /* sqrt(2^64-1) < 2^32 */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        /* mid*mid may overflow, so test via division */
        if (mid > 0 && mid > x / mid) {
            high = mid - 1;
        } else {
            low = mid;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Any n < 5 cannot be written as x^2 + y^2 with x!=y>0 */
    if (n < 5) {
        return (seq_t)0;
    }

    for (idx_t x = 1; (idx_t)x * x <= n; ++x) {
        idx_t y_sq = n - x * x;
        if (y_sq == 0) {
            continue;                 /* skip square n = x^2 + 0^2 */
        }
        idx_t y = isqrt_u64(y_sq);
        if (y * y == y_sq && y != x) {
            return (seq_t)1;           /* found distinct non‑zero squares */
        }
    }
    return (seq_t)0;
}
