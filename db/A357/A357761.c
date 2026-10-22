/*
 * A357761  a(n) = A227872(n) - A356018(n).
 * https://oeis.org/A357761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = sum_{d|n} (1 if popcount(d) is odd else -1) (excess of
 *            odious over evil divisors)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   pop_parity_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pop_parity_u64.h"

/* integer square root: floor(sqrt(n)) for 0 <= n < 2^64 */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = ((idx_t)1 << 32);          /* 2^32 > sqrt(2^64-1) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid && mid > n / mid) {
            high = mid;
        } else {
            if (mid * mid > n) {
                high = mid;
            } else {
                low = mid + 1;
            }
        }
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */
    seq_t sum = 0;
    idx_t limit = isqrt_u64(n);
    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            /* divisor d */
            sum += (seq_t)(pop_parity_u64(d) ? 1 : -1);
            idx_t d2 = n / d;
            if (d2 != d) {
                /* paired divisor */
                sum += (seq_t)(pop_parity_u64(d2) ? 1 : -1);
            }
        }
    }
    return sum;
}
