/*
 * A382278  a(n) = least integer m >= 2 such that n is a sum of the form
 *          Sum_{k>=0} floor(h/m^k) for some integer h >= 1.
 * https://oeis.org/A382278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer m >= 2 such that n = Σ_{k>=0} floor(h/m^k)
 *            * for some integer h >= 1. The code implements this by *
 *            iterating over m and searching for an appropriate h using *
 *            binary search. It follows the definition directly * without any
 *            lookup tables
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* search m starting from 2 up to n+1 (m > n always works) */
    for (idx_t m = 2; m <= n + 1; ++m) {
        /* minimal possible h: ceil((m-1)*n/m) = n - floor(n/m) */
        idx_t low  = n - n / m;
        idx_t high = n;
        while (low <= high) {
            idx_t mid = low + (high - low) / 2;
            seq_t sum = 0;
            idx_t t = mid;
            /* compute S(m,mid) = Σ floor(mid/m^k) */
            while (t) {
                sum += (seq_t)t;
                t /= m;
            }
            if (sum == n) {
                return (seq_t)m;
            } else if (sum < n) {
                low = mid + 1;
            } else {
                if (mid == 0) break;   /* prevent underflow */
                high = mid - 1;
            }
        }
    }
    /* fallback: m = n+1 always works (h = n) */
    return (seq_t)(n + 1);
}
