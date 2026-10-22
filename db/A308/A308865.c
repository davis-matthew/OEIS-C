/*
 * A308865  a(n) = Sum_{k>=0} k^(2*n+1)/2^(k+1).
 * https://oeis.org/A308865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000670(2*n+1) = Sum_{k=0}^{2*n+1} k!*Stirling2(2*n+1,k)
 *            Recurrence for ordered Bell numbers: B_0 = 1 B_m =
 *            sum_{k=0}^{m-1} binom(m,k) * B_k (m ≥ 1) We need a(n) =
 *            B_{2*n+1}. The values are cached for all n in any order of calls
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* holds B[0 .. max]   */
static idx_t  cache_len = 0;         /* total allocated size */
static idx_t  cache_max = 0;         /* highest m with B_m known */

/* enlarge cache to contain values up to m (inclusive) */
static void ensure_cache(idx_t m)
{
    if (m < cache_max)
        return;

    /* grow array by realloc */
    idx_t new_len = m + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort();                     /* out of memory */
    cache = tmp;
    cache_len = new_len;

    /* initialise first element if this is the first call */
    if (cache_max == 0)
        cache[0] = (seq_t)1;

    /* compute new ordered Bell numbers from cache_max to m */
    for (idx_t i = cache_max; i <= m; ++i) {
        if (i == 0)      /* B_0 already set */
            continue;
        seq_t sum   = 0;
        seq_t binom = 1;            /* binom(i,0) = 1 */
        for (idx_t k = 0; k < i; ++k) {
            sum += binom * cache[k];
            if (k + 1 < i) {
                /* binom(i,k+1) = binom(i,k) * (i-k) / (k+1) */
                binom = (binom * (seq_t)(i - k)) / (seq_t)(k + 1);
            }
        }
        cache[i] = sum;
    }
    cache_max = m + 1;              /* now B_0 .. B_m are known */
}

seq_t a(idx_t n)
{
    /* we need B_{2*n+1} */
    idx_t m_needed = 2 * n + 1;
    ensure_cache(m_needed);
    return cache[m_needed];
}
