/*
 * A307510  a(n) is the greatest product i*j*k*l where i^2 + j^2 + k^2 + l^2 =
 *          n and 0 <= i <= j <= k <= l.
 * https://oeis.org/A307510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = max{i*j*k*l | i^2 + j^2 + k^2 + l^2 = n, 0 <= i <= j <= k <= l} */
#include <stdint.h>
#include <stdbool.h>

/* integer square‑root for unsigned 64‑bit values, returns floor(sqrt(n)) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid != 0 && mid > UINT64_MAX / mid) {
            /* mid^2 would overflow, so treat as > n */
            high = mid - 1;
            continue;
        }
        idx_t sq = mid * mid;
        if (sq == n) {
            return mid;
        } else if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    seq_t max_prod = 0;
    idx_t limit_i = isqrt_u64(n);
    for (idx_t i = 0; i <= limit_i; ++i) {
        idx_t sq_i = i * i;
        if (sq_i > n) break;
        idx_t remain1 = n - sq_i;
        idx_t limit_j = isqrt_u64(remain1);
        for (idx_t j = i; j <= limit_j; ++j) {
            idx_t sq_j = j * j;
            if (sq_j > remain1) break;
            idx_t remain2 = remain1 - sq_j;
            idx_t limit_k = isqrt_u64(remain2);
            for (idx_t k = j; k <= limit_k; ++k) {
                idx_t sq_k = k * k;
                if (sq_k > remain2) break;
                idx_t remain3 = remain2 - sq_k;
                /* if remain3 < k^2, no l >= k can satisfy l^2 = remain3 */
                if (remain3 < sq_k) break;
                idx_t l = isqrt_u64(remain3);
                if (l * l != remain3) continue;
                if (l < k) continue;
                seq_t prod = ((seq_t)i * (seq_t)j * (seq_t)k * (seq_t)l);
                if (prod > max_prod) max_prod = prod;
            }
        }
    }
    return max_prod;
}
