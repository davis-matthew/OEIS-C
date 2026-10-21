/*
 * A296666  Table read by rows, the even rows of A296664, T(n, k) for n >= 0
 *          and 0 <= k <= 2n.
 * https://oeis.org/A296666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where r = floor(sqrt(n)), k = n - r². For 0 ≤ k ≤
 *            r: T = binomial(2r,r) - binomial(2r,r+k+1) For r < k ≤ 2r: T =
 *            binomial(2r,r) - binomial(2r,k-r-1)
 * Range:     u8 n <= 15, u16 n <= 100, u32 n <= 261, u64 n <= 960,
 *            u128 n <= 960, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static inline idx_t floor_isqrt_u64(idx_t v)
{
    idx_t a = v;
    idx_t b = (a + 1) >> 1;
    while (b < a) {
        a = b;
        b = (a + v / a) >> 1;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Determine the row r and column k of the flattened triangle. */
    idx_t r = floor_isqrt_u64(n);             /* floor(sqrt(n)) */
    idx_t start = r * r;                      /* index of first element in row r */
    idx_t k = n - start;                      /* position within the row */

    idx_t two_r = r << 1;
    seq_t base = binomial(two_r, r);          /* C(2r,r) */

    if (k <= r) {
        /* k ≤ r:   T = C(2r,r) - C(2r,r+k+1)   (subscript may exceed 2r) */
        idx_t sub_idx = r + k + 1;
        seq_t sub = (sub_idx > two_r) ? 0 : binomial(two_r, sub_idx);
        return base - sub;
    } else {
        /* k > r:  T = C(2r,r) - C(2r,k-r-1)   (subscript ≤ 2r) */
        idx_t sub_idx = k - r - 1;
        seq_t sub = binomial(two_r, sub_idx);
        return base - sub;
    }
}
