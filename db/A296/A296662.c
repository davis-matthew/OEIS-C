/*
 * A296662  Table read by rows, the odd rows of A296664, T(n, k) for n >= 0
 *          and 0 <= k <= 2n.
 * https://oeis.org/A296662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where r = floor(sqrt(n)), k = n - r², and T(n,k)
 *            is defined by T(n,k) = C(2n+1,n+1) - C(2n+1, n-k-1) for k ≤ n,
 *            T(n,k) = C(2n+1,n+1) - C(2n+1, k-n-1) for k > n
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Compute floor(sqrt(x)) for 0 ≤ x < 2⁶⁴. */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = 1ULL << 32;            /* sqrt(2^64-1) < 2^32 */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (mid * mid <= x) {
            low = mid + 1;              /* mid is still a candidate */
        } else {
            high = mid;                 /* mid is too big */
        }
    }
    return low - 1;                     /* last value that satisfies mid² ≤ x */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;           /* row 0 has a single entry 1 */

    idx_t r = floor_sqrt_u64(n);            /* row index */
    idx_t k = n - r * r;                    /* position within the row */

    idx_t base_n = 2 * r + 1;
    idx_t base_k = r + 1;
    seq_t base = binomial(base_n, base_k);

    seq_t sub;
    if (k <= r) {
        idx_t sub_k = r - k - 1;
        sub = binomial(base_n, sub_k);
    } else {
        idx_t sub_k = k - r - 1;
        sub = binomial(base_n, sub_k);
    }
    return base - sub;
}
