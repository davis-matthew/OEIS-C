/*
 * A359503  Partial sums of A066839.
 * https://oeis.org/A359503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d=1}^{⌊√n⌋} d · (⌊n/d⌋ – d + 1)
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th partial sum of A066839: sum of all divisors d of k such that d² ≤ k, for k = 1…n. */
#include <stdint.h>

/* Integer floor‑square‑root (works for any idx_t). */
static inline idx_t floor_sqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {         /* avoid division by zero when n = 0 */
            low = mid + 1;
            continue;
        }
        if (mid <= n / mid)     /* mid² ≤ n */
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;                 /* floor(√n) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset starts at 1 – n = 0 is not used */
    idx_t m = floor_sqrt_idx(n);
    seq_t sum = 0;
    for (idx_t d = 1; d <= m; ++d) {
        idx_t cnt = n / d - d + 1;                /* how many k have this d */
        sum += (seq_t)d * (seq_t)cnt;             /* accumulate contribution */
    }
    return sum;
}
