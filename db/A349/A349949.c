/*
 * A349949  a(n) is the number of divisors of n that are 1 above or 1 below a
 *          divisor of either n+1 or n-1.
 * https://oeis.org/A349949
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts divisors d of n for which either d-1 or d+1 *
 *            divides n-1 or n+1. No direct OEIS formula is available; * we
 *            compute it by enumerating divisors of n
 * Range:     u8 n <= 9296, u16 n <= 9296, u32 n <= 9296, u64 n <= 9296,
 *            u128 n <= 9296, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool good_divisor(idx_t d, idx_t n) {
    /* d-1 must be >0 */
    if (d > 1) {
        idx_t dm1 = d - 1;
        if ((n + 1) % dm1 == 0 || (n - 1) % dm1 == 0) {
            return true;
        }
    }
    /* d+1 must be <= n+1 to be a divisor of n+1 or n-1 */
    /* but if d+1 > n+1 then it cannot divide n+1 or n-1, so we skip */
    idx_t dp1 = d + 1;
    if (dp1 <= n + 1) {
        if ((n + 1) % dp1 == 0 || (n - 1) % dp1 == 0) {
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    if (n < 2) return 0; /* offset starts at 2, behaviour below not important */

    seq_t count = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            if (good_divisor(d, n)) ++count;
            idx_t d2 = n / d;
            if (d2 != d) {
                if (good_divisor(d2, n)) ++count;
            }
        }
    }
    return count;
}
