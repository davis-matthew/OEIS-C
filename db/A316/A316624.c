/*
 * A316624  Number of balanced p-trees with n leaves.
 * https://oeis.org/A316624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of balanced p‑trees with n leaves. Computed via
 *            recurrence on heights, storing all values in a cache.
 *            Complexity: O(log2(n) * n^2), acceptable for n ≤ 10000
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 53, u64 n <= 100,
 *            u128 n <= 194, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External types supplied by the build. */
typedef uint64_t idx_t;   /* assumed; replace with actual type if different */

/* Cache structure */
static seq_t *a_cache = NULL;      /* array of a(0..N) */
static size_t  cache_cap = 0;      /* array size */

/* Ensure cache large enough and recompute if necessary. */
static void recompute_up_to(idx_t limit)
{
    if ((size_t)limit + 1 <= cache_cap)
        return;                     /* already computed up to this limit */

    size_t N = (size_t)limit;
    /* Allocate new cache, zero‑initialized */
    seq_t *new_cache = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!new_cache) return;            /* out of memory; caller will handle */

    /* Base case for height 0 */
    if (N >= 1) new_cache[1] = (seq_t)1;

    /* Arrays for dynamic programming */
    seq_t *prev = (seq_t*)calloc(N + 1, sizeof(seq_t));      /* B[h-1][n]  */
    seq_t *dp   = NULL;                                      /* temporary DP */

    /* height 0 tree */
    prev[1] = (seq_t)1;                                      /* B[0][1] = 1 */

    for (size_t h = 1; ; ++h)
    {
        /* Compute dp[n] = coefficient of x^n in ∏_{w} (1 - prev[w] x^w)^-1 */
        dp = (seq_t*)calloc(N + 1, sizeof(seq_t));
        if (!dp) break;        /* memory failure; break and keep current cache */
        dp[0] = (seq_t)1;

        for (size_t w = 1; w <= N; ++w)
        {
            seq_t val = prev[w];
            if (val == (seq_t)0) continue;
            for (size_t n = w; n <= N; ++n)
            {
                dp[n] += (dp[n - w] * val);
            }
        }

        bool any_nonzero = false;

        for (size_t n = 1; n <= N; ++n)
        {
            seq_t cur = dp[n] - prev[n];   /* exclude single‑part partitions */
            if (cur != (seq_t)0)
            {
                new_cache[n] += cur;       /* add to total a(n) */
                prev[n] = cur;             /* prepare for next height */
                any_nonzero = true;
            }
            else
            {
                prev[n] = cur;             /* zero remains zero */
            }
        }

        free(dp);

        if (!any_nonzero) break;   /* no new trees at this height */
    }

    free(prev);

    /* Replace old cache */
    if (a_cache) free(a_cache);
    a_cache    = new_cache;
    cache_cap  = N + 1;
}

/* Main public function. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;  /* undefined for n ≤ 0 */
    recompute_up_to(n);
    return a_cache[(size_t)n];
}
