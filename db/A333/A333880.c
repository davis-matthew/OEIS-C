/*
 * A333880  Number of integer solutions to n = x^k - y^k with x > y >= 0 and k
 *          >= 2.
 * https://oeis.org/A333880
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y,k) | n = x^k - y^k, x>y>=0, k>=2}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* fast exponentiation using idx_t (uint64_t) */
static inline idx_t pow_idx(idx_t base, idx_t exp) {
    idx_t result = 1;
    while (exp) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* binary search for y (0 ≤ y ≤ max_y) such that y^k == val */
static inline idx_t kth_root_val(idx_t val, idx_t k, idx_t max_y) {
    idx_t low = 0, high = max_y, found = (idx_t)-1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t midk = pow_idx(mid, k);
        if (midk == val) {
            found = mid;
            break;
        } else if (midk < val) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return found;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;

    idx_t count = 0;
    idx_t two_k = 2;          /* 2^k, start with k = 2 */

    for (idx_t k = 2; ; ++k) {
        if (two_k > n + 1) break;   /* 2^k - 1 > n → no further k */

        /* iterate over possible x */
        for (idx_t x = 1; ; ++x) {
            idx_t xk = pow_idx(x, k);
            if (xk < n) continue;   /* difference would be negative */

            idx_t prev_k = pow_idx(x - 1, k);
            idx_t diff_min = xk - prev_k;   /* minimal difference for this x */
            if (diff_min > n) break;        /* larger x will only increase diff */

            idx_t base = xk - n;            /* y^k must equal this value */
            idx_t y = kth_root_val(base, k, x - 1);
            if (y != (idx_t)-1 && y < x) count++;
        }

        two_k <<= 1;   /* advance to next power of two */
    }

    return (seq_t)count;
}
