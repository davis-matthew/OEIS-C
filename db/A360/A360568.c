/*
 * A360568  Number of divisors d of n such that n - d is not square.
 * https://oeis.org/A360568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that n - d is not a
 *            perfect square
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* idx_t is unsigned 64‑bit; seq_t is an unsigned integer type (up to 128‑bit) */

/* Integer square root: floor(sqrt(n)) using binary search and 128‑bit arithmetic. */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    idx_t res = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* avoid underflow */
            high = mid - 1;
        }
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) return 0;   /* safety, although offset starts at 1 */

    idx_t limit = floor_isqrt_u64(n);

    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            idx_t m1 = n - d;
            idx_t r1 = floor_isqrt_u64(m1);
            if (r1 * r1 != m1) {
                count++;
            }

            /* complementary divisor n/d, if distinct */
            if (d != n / d) {
                idx_t d2 = n / d;
                idx_t m2 = n - d2;
                idx_t r2 = floor_isqrt_u64(m2);
                if (r2 * r2 != m2) {
                    count++;
                }
            }
        }
    }

    return count;
}
