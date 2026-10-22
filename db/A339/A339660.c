/*
 * A339660  Number of strict integer partitions of n with no 1's and a part
 *          divisible by all the other parts.
 * https://oeis.org/A339660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A339660:
     Number of strict integer partitions of n with no 1's and a part
     divisible by all the other parts.
     The algorithm uses for each largest part L (2 ≤ L ≤ n) a subset‑sum DP
     over the divisors of L > 1, counting every subset of those divisors that
     sums to a value s.  The partition contributes to a[n] when n = L + s.
*/

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build system */
extern seq_t a(idx_t n)
{
    /* Static cache of the sequence values up to the largest n computed so far */
    static idx_t cache_cap   = 0;          /* current capacity of cache   */
    static seq_t *cache      = NULL;       /* array holding a(0)…a(max)   */

    /* Expand the cache if the requested n is outside the current bounds */
    if (n + 1 > cache_cap) {
        /* New cache size */
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)calloc((size_t)new_cap, sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed – return 0 as graceful failure */

        /* a(0) = 1 (empty partition) */
        new_cache[0] = 1;

        /* Working array for subset sum DP, size n+1 */
        seq_t *dp = (seq_t *)calloc((size_t)new_cap, sizeof(seq_t));
        if (!dp) {
            free(new_cache);
            return 0;
        }

        /* Temporary array to hold divisors of a given L */
        idx_t divs[128];
        size_t divcnt;

        /* Main loop over possible largest part L */
        for (idx_t L = 2; L <= n; ++L) {
            idx_t smax = n - L;                /* maximum sum of the remaining parts */

            /* Reset DP for this L */
            for (idx_t i = 0; i <= smax; ++i) dp[i] = 0;
            dp[0] = 1;                         /* empty subset */

            /* Compute divisors d of L that are >1 and <L */
            divcnt = 0;
            for (idx_t d = 2; d * d <= L; ++d) {
                if (L % d == 0) {
                    divs[divcnt++] = d;
                    idx_t other = L / d;
                    if (other != d && other != L) divs[divcnt++] = other;
                }
            }

            /* Subset sum DP over the divisors */
            for (size_t idx = 0; idx < divcnt; ++idx) {
                idx_t d = divs[idx];
                if (d > smax) continue;
                for (idx_t s = smax; s >= d; --s) {
                    dp[s] += dp[s - d];
                }
            }

            /* Add this L's contributions to the cache */
            for (idx_t s = 0; s <= smax; ++s) {
                idx_t target = L + s;
                new_cache[target] += dp[s];
            }
        }

        /* Clean up and install the new cache */
        free(dp);
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    return cache[n];
}
