/*
 * A250028  a(n) is the number of positive integers k <= n such that lpf(k^2 +
 *          1) = lpf(n^2 + 1), where lpf() is the least prime factor function.
 * https://oeis.org/A250028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k ≤ n | lpf(k^2 + 1) = lpf(n^2 + 1) }| (where lpf(m)
 *            is the least prime factor of m)
 * Range:     u8 n <= 17, u16 n <= 783, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/lpf.h"

/* static cache of least prime factors for k^2+1, indexed by k-1 */
static idx_t *lpf_cache = NULL;   /* array of lpf values */
static idx_t  cache_len = 0;      /* number of entries currently stored */

/* helper: extend cache to accommodate all k up to n */
static void extend_cache(idx_t n)
{
    if (n <= cache_len) return;
    idx_t new_len = n;
    idx_t *new_cache;
    if (lpf_cache == NULL)
        new_cache = (idx_t *)malloc(new_len * sizeof(idx_t));
    else
        new_cache = (idx_t *)realloc(lpf_cache, new_len * sizeof(idx_t));
    if (new_cache == NULL) abort();      /* allocation failure */
    /* compute lpf for new entries */
    for (idx_t k = cache_len + 1; k <= new_len; ++k)
    {
        /* compute k^2 + 1 safely in seq_t, then cast to idx_t for lpf */
        seq_t val = (seq_t)k * (seq_t)k + 1;
        new_cache[k - 1] = lpf((idx_t)val);
    }
    lpf_cache = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; n=0 is outside the sequence */
    /* ensure cache contains lpf for all k ≤ n */
    extend_cache(n);
    idx_t target_lpf = lpf_cache[n - 1];  /* lpf of n^2+1 */
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k)
        if (lpf_cache[k - 1] == target_lpf)
            ++count;
    return count;
}
