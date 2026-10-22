/*
 * A364387  Number of divisors of n of the form 4*k+3 that are at most
 *          sqrt(n).
 * https://oeis.org/A364387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of divisors of n of the form 4*k+3 that are at most
 *            sqrt(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* not used, offset starts at 1 */
    /* Integer square root of n (floor). */
    idx_t sqrt_n = 0;
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid == 0) {          /* protect from division by zero */
            sqrt_n = 0;
            lo = 1;
            continue;
        }
        if (mid <= n / mid) {
            sqrt_n = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    seq_t count = 0;
    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            if ((d & 3u) == 3u) /* d ≡ 3 (mod 4) */
                ++count;
        }
    }
    return count;
}
