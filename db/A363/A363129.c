/*
 * A363129  Number of integer partitions of n with a unique non-co-mode.
 * https://oeis.org/A363129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A363129
    Number of integer partitions of n with a unique non‑co‑mode.
    The algorithm enumerates the common multiplicity a, the
    extra multiplicity d, and the distinguished part size x.
    For each combination it uses partitions into distinct parts.
    Complexity: O(n²) with n up to ~10⁴, well below the 2‑second limit. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 4) return 0;            /* a(0)=a(1)=a(2)=a(3)=0 */

    size_t N = (size_t)n;

    /* partitions into distinct parts: q[s] = number of partitions of s */
    seq_t *q = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!q) return 0;
    q[0] = 1;
    for (size_t i = 1; i <= N; ++i) q[i] = 0;
    for (size_t part = 1; part <= N; ++part)
        for (size_t sum = N; sum >= part; --sum)
            q[sum] = q[sum] + q[sum - part];

    seq_t *Qex = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!Qex) { free(q); return 0; }

    seq_t ans = 0;

    for (idx_t x = 1; x <= n; ++x) {
        /* compute Qex[m] = partitions of m into distinct parts excluding part x */
        Qex[0] = 1;
        for (size_t m = 1; m <= N; ++m) {
            if (m < (size_t)x)
                Qex[m] = q[m];
            else
                Qex[m] = q[m] - Qex[m - x];
        }

        /* iterate over possible extra multiplicity d */
        if (x > n) continue;                 /* safety, though x<=n here */
        size_t max_d = (n > 0) ? ((n - 1) / x) : 0;
        for (size_t d = 1; d <= max_d; ++d) {
            size_t t = n - d * x;            /* t = a * sumS */
            /* find all divisors a of t */
            for (size_t k = 1; k * k <= t; ++k) {
                if (t % k != 0) continue;
                size_t a1 = k;
                size_t sumS1 = t / a1;
                if (sumS1 >= (size_t)x) {
                    size_t residual = sumS1 - (size_t)x;
                    if (residual > 0)
                        ans = ans + Qex[residual];
                }
                size_t a2 = t / k;
                if (a2 != a1) {
                    size_t sumS2 = k;           /* since sumS2 = t / a2 = k */
                    if (sumS2 >= (size_t)x) {
                        size_t residual = sumS2 - (size_t)x;
                        if (residual > 0)
                            ans = ans + Qex[residual];
                    }
                }
            }
        }
    }

    free(q);
    free(Qex);
    return ans;
}
