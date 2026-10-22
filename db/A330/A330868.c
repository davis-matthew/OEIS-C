/*
 * A330868  Number of proper divisors d of n such that n-d is squarefree.
 * https://oeis.org/A330868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = Sum_{d|n, d<n} mu(n-d)^2
 * Range:     u8 n <= 507, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of proper divisors d of n such that n-d is squarefree. */
#include <stdbool.h>
#include "../utils/is_squarefree.h"

static idx_t isqrt(idx_t n) {
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid <= n / mid) {          /* mid*mid <= n without overflow */
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;                 /* no proper divisors for n = 0 or 1 */

    seq_t cnt = 0;
    idx_t root = isqrt(n);

    for (idx_t i = 1; i <= root; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            if (d1 < n) {
                seq_t m = (seq_t)n - (seq_t)d1;
                if (is_squarefree(m)) cnt++;
            }
            idx_t d2 = n / i;
            if (d2 != d1 && d2 < n) {
                seq_t m = (seq_t)n - (seq_t)d2;
                if (is_squarefree(m)) cnt++;
            }
        }
    }

    return cnt;
}
