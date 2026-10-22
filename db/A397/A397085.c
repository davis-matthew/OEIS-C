/*
 * A397085  a(n) = (-1)^(n-floor(sqrt(n))^2-1) if n is not a perfect square
 *          and 0 otherwise.
 * https://oeis.org/A397085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n - floor(sqrt(n))^2 - 1) if n is not a perfect
 *            square; 0 otherwise
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    /* Computes floor(sqrt(n)) for 0 <= n <= UINT64_MAX */
    idx_t low = 0;
    idx_t high = 0xFFFFFFFFULL;   /* (2^32 - 1) is the maximum integer sqrt of uint64_t */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq = mid * mid;
        if (sq == n) {
            return mid;
        }
        if (sq < n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* prevent underflow */
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    idx_t r = isqrt_u64(n);
    if (r * r == n)       /* perfect square → 0 */
        return 0;
    idx_t diff = n - r * r - 1;
    /* (-1)^diff returns 1 if diff even, -1 if diff odd */
    return (diff & 1) ? (seq_t)-1 : (seq_t)1;
}
