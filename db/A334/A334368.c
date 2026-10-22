/*
 * A334368  Number of proper divisors d of n such that d, n/d and n-d are all
 *          squarefree.
 * https://oeis.org/A334368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{d|n, d<n} μ(d)² • μ(n/d)² • μ(n-d)² (i.e. count the
 *            proper divisors d of n such that d, n/d and n-d are all
 *            square‑free)
 * Range:     u8 n <= 774, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;

    seq_t count = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;          /* second divisor in the pair */

            /* consider divisor d, provided it is proper */
            if (d < n) {
                if (is_squarefree((seq_t)d) &&
                    is_squarefree((seq_t)d2) &&
                    is_squarefree((seq_t)(n - d))) {
                    ++count;
                }
            }

            /* consider the complementary divisor d2 (if distinct) */
            if (d2 != d && d2 < n) {
                if (is_squarefree((seq_t)d2) &&
                    is_squarefree((seq_t)d) &&
                    is_squarefree((seq_t)(n - d2))) {
                    ++count;
                }
            }
        }
    }
    return count;
}
