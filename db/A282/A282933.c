/*
 * A282933  Number of ways to write n as x^4 + y^2 + z^2 + w^2 with x,y,z
 *          nonnegative integers and w a positive integer such that 8*y^2 -
 *          8*y*z + 9*z^2 is a square.
 * https://oeis.org/A282933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (x,y,z,w) ∈ ℕ^4 with * x^4 + y^2 +
 *            z^2 + w^2 = n, * w > 0, and 8·y^2 − 8·y·z + 9·z^2 is a perfect
 *            square. * This function computes a(n) exactly for every n ≥ 1
 * Range:     u8 n <= 36, u16 n <= 7624, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root using binary search, safe for arbitrary seq_t. */
static inline seq_t isqrt_seq(seq_t n)
{
    seq_t low = 0, high = n, ans = 0;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        /* Avoid overflow: mid * mid <= n  <=>  mid <= n / mid (mid>0) */
        if (mid == 0 || mid <= n / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Test whether a seq_t value is a perfect square. */
static inline bool is_square(seq_t v)
{
    seq_t s = isqrt_seq(v);
    return s * s == v;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; for completeness, return 0 */
        return 0;

    seq_t total = 0;
    seq_t n_seq  = (seq_t)n;

    for (idx_t x = 0; ; ++x) {
        seq_t x2 = (seq_t)x * (seq_t)x;
        seq_t x4 = x2 * x2;          /* x^4 */
        if (x4 > n_seq)
            break;

        seq_t rem_x = n_seq - x4;
        seq_t y_max = isqrt_seq(rem_x);

        for (idx_t y = 0; y <= y_max; ++y) {
            seq_t y2       = (seq_t)y * (seq_t)y;   /* y^2 */
            seq_t rem_xy   = rem_x - y2;
            seq_t z_max    = isqrt_seq(rem_xy);

            for (idx_t z = 0; z <= z_max; ++z) {
                seq_t z2       = (seq_t)z * (seq_t)z;   /* z^2 */
                seq_t rem      = rem_xy - z2;           /* remaining for w^2 */

                if (rem == 0)               /* w must be positive */
                    continue;

                seq_t w = isqrt_seq(rem);
                if (w == 0 || w * w != rem)   /* not a perfect square */
                    continue;

                seq_t q = (seq_t)8 * y2 - (seq_t)8 * y * z + (seq_t)9 * z2;
                if (is_square(q))
                    ++total;
            }
        }
    }

    return total;
}
