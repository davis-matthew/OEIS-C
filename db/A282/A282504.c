/*
 * A282504  Expansion of 1/(1 - Sum_{k>=0} x^(2*k*(k+1)+1)).
 * https://oeis.org/A282504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=∑_{k≥0, 2k(k+1)+1≤n} a(n-(2k(k+1)+1))
 * Range:     u8 n <= 21, u16 n <= 40, u32 n <= 78, u64 n <= 154,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* counts compositions into centered square numbers (1,5,13,25,…) */

#include <stdlib.h>
#include <stdint.h>

/* static cache for memoisation: cache[i] = a(i) */
static seq_t *cache = NULL;        /* array storing sequence values   */
static size_t cache_size = 0;      /* number of cached entries (max n+1) */

seq_t a(idx_t n)
{
    /* initialise cache with a(0) if empty */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* allocation failed */
        cache[0] = (seq_t)1;                /* a(0)=1 */
        cache_size = 1;
    }

    /* extend cache up to n if needed */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;         /* allocation failed */
        cache = tmp;

        /* compute new values */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; ; ++k) {
                /* centered square number: 2*k*(k+1)+1 */
                idx_t c = 2ULL * k * (k + 1ULL) + 1ULL;
                if (c > i) break;
                sum += cache[i - c];
            }
            cache[i] = sum;
        }
        cache_size = new_size;
    }

    return cache[n];
}
