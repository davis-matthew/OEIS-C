/*
 * A320163  Number of integer solutions to a^2 + 2*b^2 + 5*c^2 + 6*d^2 = n.
 * https://oeis.org/A320163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to a^2 + 2*b^2 + 5*c^2 +
 *            6*d^2 = n
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* integer floor square root of an unsigned 64‑bit integer */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid == 0 || mid <= n / mid) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Storage for counts of (a,b) and (c,d) contributions. */
    idx_t size = n + 1;
    idx_t *ab = (idx_t *)calloc((size_t)size, sizeof(idx_t));
    idx_t *cd = (idx_t *)calloc((size_t)size, sizeof(idx_t));
    if (!ab || !cd) {
        if (ab) free(ab);
        if (cd) free(cd);
        return 0; /* memory allocation failed */
    }

    /* Count contributions of a^2 + 2*b^2. */
    idx_t maxA = floor_sqrt_u64(n);
    idx_t maxB = floor_sqrt_u64(n / 2);
    for (idx_t a_val = 0; a_val <= maxA; ++a_val) {
        idx_t a2 = a_val * a_val;
        idx_t mult_a = (a_val == 0) ? 1 : 2;
        for (idx_t b_val = 0; b_val <= maxB; ++b_val) {
            idx_t b2 = b_val * b_val;
            idx_t sum = a2 + 2 * b2;
            if (sum > n) break;
            idx_t mult_b = (b_val == 0) ? 1 : 2;
            ab[sum] += mult_a * mult_b;
        }
    }

    /* Count contributions of 5*c^2 + 6*d^2. */
    idx_t maxC = floor_sqrt_u64(n / 5);
    idx_t maxD = floor_sqrt_u64(n / 6);
    for (idx_t c_val = 0; c_val <= maxC; ++c_val) {
        idx_t c2 = c_val * c_val;
        idx_t mult_c = (c_val == 0) ? 1 : 2;
        idx_t base = 5 * c2;
        for (idx_t d_val = 0; d_val <= maxD; ++d_val) {
            idx_t d2 = d_val * d_val;
            idx_t sum = base + 6 * d2;
            if (sum > n) break;
            idx_t mult_d = (d_val == 0) ? 1 : 2;
            cd[sum] += mult_c * mult_d;
        }
    }

    /* Convolution of the two arrays gives the total count. */
    seq_t result = 0;
    for (idx_t s = 0; s <= n; ++s) {
        if (ab[s] == 0 || cd[n - s] == 0) continue;
        seq_t add = (seq_t)ab[s] * (seq_t)cd[n - s];
        result += add;
    }

    free(ab);
    free(cd);
    return result;
}
