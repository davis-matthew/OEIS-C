/*
 * A325181  Number of integer partitions of n such that the difference between
 *          the length of the minimal square containing and the maximal square
 *          contained in the Young diagram is 1.
 * https://oeis.org/A325181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) counts the integer partitions of n whose Durfee square has
 *            side length k and whose minimal bounding square has side length
 *            k+1. That is, max(part1, number_of_parts) = k+1 and λ_k ≥ k ≥
 *            λ_{k+1}
 * Range:     u8 n <= 150, u16 n <= 150, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Integer square‑root floor, works for 64‑bit unsigned indices. */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (mid > 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Return the a(n) value for sequence A325181. */
seq_t a(idx_t n) {
    seq_t ans = 0;
    idx_t sqrt_n = isqrt_u64(n);               /* floor(sqrt(n)) */

    for (idx_t k = 1; k <= sqrt_n; ++k) {
        idx_t k_sq = k * k;                     /* k^2, fits into idx_t */
        if (k_sq > n) break;                    /* safety check */

        idx_t r = n - k_sq;                     /* r = n - k^2, >= 0 */
        if (r == 0) continue;                   /* excludes the (t=0,m=0) case */

        idx_t two_k = 2 * k;
        if (r > two_k) continue;                /* r must be <= 2k */

        idx_t lower_t = (r <= k) ? 0 : r - k;    /* max(0, r-k) */
        idx_t upper_t = (r <= k) ? r : k;        /* min(k, r) */

        /* number of integer t such that lower_t <= t <= upper_t */
        idx_t count_k = upper_t - lower_t + 1;
        ans += (seq_t)count_k;
    }

    return ans;
}
