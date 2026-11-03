/*
 * A320435  Regular triangle read by rows where T(n,k) is the number of
 *          relatively prime k-subsets of {1,...,n}, 1 <= k <= n.
 * https://oeis.org/A320435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k), * where m is the smallest integer such that
 *            m*(m+1)/2 ≥ n, * and k = n - (m-1)*m/2. T(m,k) = Σ_{d=1}^{⌊m/k⌋}
 *            μ(d)·C(⌊m/d⌋, k)
 * Range:     u8 n <= 58, u16 n <= 178, u32 n <= 611, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Validated: standard, pass, 85 terms through n = 1199 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: μ(d) is the Möbius function; C(n,k) is the binomial coefficient. */

/* Required utility functions */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

/* Compute a(n) as defined above. */
seq_t a(idx_t n) {
    /* Find row m by binary search on m*(m+1)/2 >= n */
    idx_t low = 0;
    idx_t high = 1;
    while (((__uint128_t)high * (high + 1)) / 2 < n) {
        high <<= 1;
    }
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__uint128_t)mid * (mid + 1)) / 2 >= n) {
            high = mid;
        } else {
            low = mid;
        }
    }
    idx_t m = high;

    /* Column k in the row m */
    idx_t prev_total = ((idx_t)((__uint128_t)(m - 1) * m / 2));
    idx_t k = n - prev_total;                /* 1 ≤ k ≤ m */

    /* Calculate T(m,k) = Σ μ(d)·C(⌊m/d⌋, k) for d = 1 .. ⌊m/k⌋ */
    seq_t result = 0;
    idx_t max_d = m / k;
    for (idx_t d = 1; d <= max_d; ++d) {
        int mu = mu_of(d);
        if (mu == 0) continue;
        seq_t bin = binomial(m / d, k);
        result += (seq_t)mu * bin;
    }
    return result;
}
