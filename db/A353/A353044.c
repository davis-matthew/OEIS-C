/*
 * A353044  a(n) is the minimal sum of squares over partitions of n with a
 *          nonnegative rank.
 * https://oeis.org/A353044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the minimal sum of squares over partitions of n * with a
 *            nonnegative partition rank (largest part ≥ number of parts)
 * Range:     u8 n <= 15, u16 n <= 539, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, offset starts at 1 */
    idx_t maxK = (n + 1) / 2;      /* only k ≤ (n+1)/2 can satisfy n+1 ≥ 2k */
    seq_t best = 0;
    bool first = true;

    for (idx_t k = 1; k <= maxK; ++k) {
        /* ceil(n/k) */
        idx_t t = (n + k - 1) / k;
        seq_t S;

        if (t >= k) {               /* even distribution is feasible */
            idx_t q = n / k;          /* floor(n/k)  */
            idx_t r = n % k;          /* number of parts that get +1 */

            seq_t qsq  = (seq_t)q * (seq_t)q;
            seq_t qp1sq = (seq_t)(q + 1) * (seq_t)(q + 1);
            S = (seq_t)r * qp1sq + (seq_t)(k - r) * qsq;
        } else {                     /* largest part forced to be k */
            idx_t rem = n - k;        /* sum of the remaining k‑1 parts */
            idx_t m   = k - 1;        /* number of those parts */

            /* rem ≥ m holds because n+1 ≥ 2k in this branch */
            idx_t q = rem / m;        /* floor(rem/m)  */
            idx_t r = rem % m;        /* number of parts that get +1 */

            seq_t qsq  = (seq_t)q * (seq_t)q;
            seq_t qp1sq = (seq_t)(q + 1) * (seq_t)(q + 1);
            seq_t kk = (seq_t)k * (seq_t)k;

            S = kk + (seq_t)r * qp1sq + (seq_t)(m - r) * qsq;
        }

        if (first || S < best) {
            best = S;
            first = false;
        }
    }

    return best;
}
