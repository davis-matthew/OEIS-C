/*
 * A307520  Expansion of Product_{k>=1} ((1 - x)^k - x^k)/((1 - x)^k + x^k).
 * https://oeis.org/A307520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = 2 * Σ_{k=1..⌊√n⌋} (-1)^k *
 *            binomial(n‑1, k²‑1)
 * Range:     i8 n <= 8, i16 n <= 15, i32 n <= 30, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Integer square‑root for 64‑bit unsigned integers */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    idx_t ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
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
    if (n == 0) return 1;          /* a(0) = 1 */
    seq_t sum = 0;
    idx_t kmax = isqrt_u64(n);
    for (idx_t k = 1; k <= kmax; ++k) {
        /* compute k^2 - 1 safely */
        unsigned __int128 ksq = (unsigned __int128)k * k;
        idx_t ksq_minus1 = (idx_t)(ksq - 1);
        seq_t term = binomial(n - 1, ksq_minus1);
        seq_t sign = (k & 1) ? (seq_t)(-1) : (seq_t)1;
        sum += sign * term;
    }
    return sum * 2;
}
